#include "archive_manager.h"

#include <archive.h>
#include <archive_entry.h>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

namespace {

struct ArchiveDeleter {
    void operator()(archive* value) const {
        if (value) {
            archive_read_free(value);
        }
    }
};

using ArchivePtr =
    std::unique_ptr<archive, ArchiveDeleter>;

ArchivePtr OpenArchive(
    const std::string& path,
    std::string& error) {

    ArchivePtr reader(archive_read_new());

    if (!reader) {
        error = "Cannot allocate archive reader";
        return {};
    }

    archive_read_support_filter_all(reader.get());
    archive_read_support_format_zip(reader.get());
    archive_read_support_format_tar(reader.get());

    if (archive_read_open_filename(
            reader.get(), path.c_str(), 10240)
        != ARCHIVE_OK) {

        const char* message =
            archive_error_string(reader.get());

        error = message
            ? message
            : "Cannot open archive";

        return {};
    }

    return reader;
}

std::string Lower(std::string value) {
    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });

    return value;
}

bool SupportedExtension(const fs::path& path) {
    const auto extension =
        Lower(path.extension().string());

    return extension == ".zip" ||
           extension == ".tar" ||
           extension == ".tgz" ||
           extension == ".gz";
}

// Reject absolute paths and any normalized path that
// contains a parent-directory traversal component.
bool SafeRelativePath(
    const char* raw,
    fs::path& result) {

    if (!raw || !*raw) {
        return false;
    }

    fs::path path(raw);

    if (path.is_absolute() ||
        path.has_root_name() ||
        path.has_root_directory()) {
        return false;
    }

    path = path.lexically_normal();

    if (path.empty() || path == ".") {
        result = path;
        return true;
    }

    for (const auto& component : path) {
        if (component == "..") {
            return false;
        }
    }

    result = path;
    return true;
}

bool IsAllowedEntryType(archive_entry* entry) {
    const auto type = archive_entry_filetype(entry);

    return type == AE_IFREG || type == AE_IFDIR;
}

const char* ArchiveError(archive* reader) {
    const char* message = archive_error_string(reader);

    return message ? message : "Archive operation failed";
}

} // namespace

ArchiveManager::ArchiveManager() = default;

ArchiveManager::~ArchiveManager() = default;

bool ArchiveManager::IsSupportedArchive(
    const std::string& archive_path) const {

    std::error_code ec;

    return SupportedExtension(fs::path(archive_path)) &&
           fs::is_regular_file(archive_path, ec) &&
           !ec;
}

ArchiveResult ArchiveManager::ListEntries(
    const std::string& path,
    std::vector<ArchiveEntryInfo>& out_entries) const {

    out_entries.clear();

    std::error_code ec;

    if (!fs::is_regular_file(path, ec) || ec) {
        return {
            false,
            "Archive does not exist or is not a regular file",
            0
        };
    }

    std::string error;
    auto reader = OpenArchive(path, error);

    if (!reader) {
        return {false, error, 0};
    }

    archive_entry* entry = nullptr;

    while (true) {
        const int rc =
            archive_read_next_header(reader.get(), &entry);

        if (rc == ARCHIVE_EOF) {
            break;
        }

        if (rc != ARCHIVE_OK) {
            return {
                false,
                ArchiveError(reader.get()),
                out_entries.size()
            };
        }

        fs::path relative;

        if (!SafeRelativePath(
                archive_entry_pathname(entry),
                relative)) {

            return {
                false,
                "Unsafe path found in archive",
                out_entries.size()
            };
        }

        if (!IsAllowedEntryType(entry)) {
            return {
                false,
                "Links and special files are unsupported",
                out_entries.size()
            };
        }

        const auto size = std::max<la_int64_t>(
            0, archive_entry_size(entry));

        out_entries.push_back({
            relative.generic_string(),
            static_cast<std::uint64_t>(size),
            archive_entry_filetype(entry) == AE_IFDIR
        });

        if (archive_read_data_skip(reader.get()) < ARCHIVE_OK) {
            return {
                false,
                ArchiveError(reader.get()),
                out_entries.size()
            };
        }
    }

    return {
        true,
        "Archive inspected successfully",
        out_entries.size()
    };
}

ArchiveResult ArchiveManager::Extract(
    const std::string& path,
    const std::string& destination,
    bool overwrite_existing) const {

    std::error_code ec;

    if (!fs::is_regular_file(path, ec) || ec) {
        return {
            false,
            "Archive does not exist or is not a regular file",
            0
        };
    }

    if (destination.empty()) {
        return {false, "Destination is empty", 0};
    }

    fs::create_directories(destination, ec);

    if (ec) {
        return {
            false,
            "Cannot create destination: " + ec.message(),
            0
        };
    }

    if (!fs::is_directory(destination, ec) || ec) {
        return {
            false,
            "Destination is not a directory",
            0
        };
    }

    const fs::path root = fs::canonical(destination, ec);

    if (ec) {
        return {
            false,
            "Cannot resolve destination directory",
            0
        };
    }

    std::string error;
    auto reader = OpenArchive(path, error);

    if (!reader) {
        return {false, error, 0};
    }

    archive_entry* entry = nullptr;
    std::size_t processed = 0;

    while (true) {
        const int rc =
            archive_read_next_header(reader.get(), &entry);

        if (rc == ARCHIVE_EOF) {
            break;
        }

        if (rc != ARCHIVE_OK) {
            return {
                false,
                ArchiveError(reader.get()),
                processed
            };
        }

        fs::path relative;

        if (!SafeRelativePath(
                archive_entry_pathname(entry),
                relative)) {

            return {
                false,
                "Unsafe archive path; extraction stopped",
                processed
            };
        }

        if (!IsAllowedEntryType(entry)) {
            return {
                false,
                "Links and special files are not extracted",
                processed
            };
        }

        const fs::path target =
            (root / relative).lexically_normal();

        const fs::path rel_check =
            target.lexically_relative(root);

        if (rel_check.empty() && target != root) {
            return {
                false,
                "Entry escapes destination",
                processed
            };
        }

        for (const auto& component : rel_check) {
            if (component == "..") {
                return {
                    false,
                    "Entry escapes destination",
                    processed
                };
            }
        }

        if (target == root) {
            if (archive_entry_filetype(entry) == AE_IFDIR) {
                ++processed;
                continue;
            }

            return {
                false,
                "Invalid output path",
                processed
            };
        }

        if (archive_entry_filetype(entry) == AE_IFDIR) {
            // Refuse symlinks in the destination path.
            fs::path cursor = root;

            for (const auto& component : rel_check) {
                cursor /= component;

                const auto status =
                    fs::symlink_status(cursor, ec);

                if (!ec && fs::is_symlink(status)) {
                    return {
                        false,
                        "Symlink in destination path rejected",
                        processed
                    };
                }

                ec.clear();
            }

            fs::create_directories(target, ec);

            if (ec) {
                return {
                    false,
                    "Cannot create directory: " +
                        ec.message(),
                    processed
                };
            }

            ++processed;
            continue;
        }

        // Create parent directories only after validating
        // the entry path.
        fs::path cursor = root;

        for (const auto& component : rel_check.parent_path()) {
            cursor /= component;

            const auto status =
                fs::symlink_status(cursor, ec);

            if (!ec && fs::is_symlink(status)) {
                return {
                    false,
                    "Symlink in destination path rejected",
                    processed
                };
            }

            ec.clear();
        }

        // Do not follow a symlink as the output file.
        const auto target_status =
            fs::symlink_status(target, ec);

        if (!ec && fs::is_symlink(target_status)) {
            return {
                false,
                "Symlink destination rejected",
                processed
            };
        }

        ec.clear();

        if (fs::exists(target, ec) && !overwrite_existing) {
            return {
                false,
                "Refusing to overwrite: " +
                    relative.generic_string(),
                processed
            };
        }

        if (ec) {
            return {
                false,
                "Cannot inspect destination: " +
                    ec.message(),
                processed
            };
        }

        fs::create_directories(target.parent_path(), ec);

        if (ec) {
            return {
                false,
                "Cannot create parent directory: " +
                    ec.message(),
                processed
            };
        }

        std::ofstream output(
            target,
            std::ios::binary | std::ios::trunc);

        if (!output) {
            return {
                false,
                "Cannot open output file: " +
                    relative.generic_string(),
                processed
            };
        }

        char buffer[16384];

        while (true) {
            const la_ssize_t count =
                archive_read_data(
                    reader.get(), buffer, sizeof(buffer));

            if (count == 0) {
                break;
            }

            if (count < 0) {
                return {
                    false,
                    ArchiveError(reader.get()),
                    processed
                };
            }

            output.write(
                buffer,
                static_cast<std::streamsize>(count));

            if (!output) {
                return {
                    false,
                    "Disk write failed: " +
                        relative.generic_string(),
                    processed
                };
            }
        }

        output.close();

        if (!output) {
            return {
                false,
                "Failed to finish writing: " +
                    relative.generic_string(),
                processed
            };
        }

        ++processed;
    }

    return {
        true,
        "Archive extracted successfully",
        processed
    };
}
