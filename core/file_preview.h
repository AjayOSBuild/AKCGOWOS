#pragma once

#include "ui_toolkit.h"
#include "thumbnail_generator.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

class FilePreviewPanel final : public Widget {
public:
    enum class PreviewKind {
        Empty,
        Text,
        Image,
        Hex,
        Error
    };

    explicit FilePreviewPanel(
        ThumbnailGenerator* thumbnails = nullptr)
        : thumbnails_(thumbnails) {}

    bool LoadFile(
        const std::string& path,
        std::size_t max_bytes = 64 * 1024,
        std::size_t max_lines = 200) {

        Clear();
        source_path_ = path;

        namespace fs = std::filesystem;
        std::error_code ec;

        if (!fs::is_regular_file(path, ec) || ec) {
            return Fail("Not a regular file");
        }

        const auto file_size = fs::file_size(path, ec);

        if (ec) {
            return Fail("Cannot read file metadata");
        }

        const std::string extension =
            Lower(fs::path(path).extension().string());

        // Image preview: use the existing thumbnail cache.
        if (IsImageExtension(extension)) {
            kind_ = PreviewKind::Image;

            if (thumbnails_) {
                if (!thumbnails_->HasThumbnail(path)) {
                    thumbnails_->GenerateThumbnail(path);
                }

                image_path_ =
                    thumbnails_->GetThumbnailPath(path);

                if (!fs::exists(image_path_, ec) || ec) {
                    image_path_ = path;
                }
            } else {
                image_path_ = path;
            }

            status_ = "Image: " +
                fs::path(path).filename().string();

            return true;
        }

        std::ifstream input(path, std::ios::binary);

        if (!input) {
            return Fail("Cannot open file");
        }

        const std::size_t amount =
            static_cast<std::size_t>(
                std::min<std::uintmax_t>(
                    file_size, max_bytes));

        std::vector<unsigned char> bytes(amount);

        if (amount != 0) {
            input.read(
                reinterpret_cast<char*>(bytes.data()),
                static_cast<std::streamsize>(amount));
        }

        bytes.resize(
            static_cast<std::size_t>(input.gcount()));

        const bool binary =
            std::find(bytes.begin(), bytes.end(), 0)
            != bytes.end();

        if (!binary &&
            (IsTextExtension(extension) ||
             LooksLikeText(bytes))) {

            kind_ = PreviewKind::Text;

            std::istringstream stream(
                std::string(bytes.begin(), bytes.end()));

            std::string line;

            while (lines_.size() < max_lines &&
                   std::getline(stream, line)) {

                if (!line.empty() && line.back() == '\r') {
                    line.pop_back();
                }

                lines_.push_back(line);
            }

            if (file_size > amount) {
                lines_.push_back(
                    "[Preview truncated: byte limit reached]");
            }

            if (lines_.empty()) {
                lines_.push_back("[Empty file]");
            }

            status_ = "Text: " +
                fs::path(path).filename().string();
        } else {
            kind_ = PreviewKind::Hex;

            lines_.push_back(
                "Hex: " + fs::path(path).filename().string());

            const std::size_t limit =
                std::min<std::size_t>(bytes.size(), 256);

            for (std::size_t offset = 0;
                 offset < limit;
                 offset += 16) {

                std::ostringstream row;

                row << std::hex
                    << std::setfill('0')
                    << std::setw(8)
                    << offset << "  ";

                std::string ascii;

                for (std::size_t j = 0; j < 16; ++j) {
                    if (offset + j < limit) {
                        const unsigned char c =
                            bytes[offset + j];

                        row << std::setw(2)
                            << static_cast<unsigned int>(c)
                            << ' ';

                        ascii += std::isprint(c)
                            ? static_cast<char>(c)
                            : '.';
                    } else {
                        row << "   ";
                    }
                }

                row << " |" << ascii << '|';
                lines_.push_back(row.str());
            }

            if (file_size > limit) {
                lines_.push_back(
                    "[Hex preview limited to 256 bytes]");
            }

            status_ = "Binary/unknown file";
        }

        return true;
    }

    void Clear() {
        source_path_.clear();
        image_path_.clear();
        status_.clear();
        lines_.clear();
        scroll_line_ = 0;
        kind_ = PreviewKind::Empty;
    }

    PreviewKind GetKind() const {
        return kind_;
    }

    const std::string& GetSourcePath() const {
        return source_path_;
    }

    const std::vector<std::string>& GetLines() const {
        return lines_;
    }

    std::size_t GetScrollLine() const {
        return scroll_line_;
    }

    void OnEvent(UIEvent& event) override {
        if (!visible || !is_focused ||
            event.type != UIEventType::KEY_PRESS ||
            !event.pressed) {
            return;
        }

        const std::size_t visible_lines =
            VisibleLineCount();

        if (event.key_code == 38) { // Up
            if (scroll_line_ > 0) {
                --scroll_line_;
            }
            event.handled = true;
        } else if (event.key_code == 40) { // Down
            if (scroll_line_ < lines_.size()) {
                ++scroll_line_;
            }
            ClampScroll();
            event.handled = true;
        } else if (event.key_code == 33) { // Page Up
            scroll_line_ =
                scroll_line_ > visible_lines
                ? scroll_line_ - visible_lines
                : 0;

            event.handled = true;
        } else if (event.key_code == 34) { // Page Down
            scroll_line_ += visible_lines;
            ClampScroll();
            event.handled = true;
        }
    }

    void Draw(
        Graphics2D& gfx,
        FontRenderer& font_engine) override {

        if (!visible) {
            return;
        }

        gfx.FillRect(
            x, y, width, height, 0xFFF8F9FB);

        gfx.DrawRect(
            x, y, width, height, 0xFF9AA4B2, 1);

        font_engine.DrawText(
            status_.empty() ? "File Preview" : status_,
            x + 8, y + 18, 12, 0xFF202938);

        if (kind_ == PreviewKind::Image &&
            !image_path_.empty()) {

            gfx.DrawImage(
                image_path_,
                x + 8,
                y + 28,
                std::max(1, width - 16),
                std::max(1, height - 36));

            return;
        }

        if (kind_ == PreviewKind::Empty) {
            font_engine.DrawText(
                "Select a file to preview",
                x + 8, y + 42, 12, 0xFF667085);
            return;
        }

        int draw_y = y + 38;

        const int line_height = 16;

        const std::size_t visible_lines =
            VisibleLineCount();

        for (std::size_t i = 0;
             i < visible_lines &&
             scroll_line_ + i < lines_.size();
             ++i) {

            font_engine.DrawText(
                lines_[scroll_line_ + i],
                x + 8,
                draw_y,
                11,
                kind_ == PreviewKind::Error
                    ? 0xFFB42318
                    : 0xFF202938);

            draw_y += line_height;
        }

        if (lines_.size() > visible_lines) {
            font_engine.DrawText(
                "Use Up/Down or Page Up/Down",
                x + 8,
                y + height - 8,
                10,
                0xFF667085);
        }
    }

private:
    ThumbnailGenerator* thumbnails_ = nullptr;

    PreviewKind kind_ = PreviewKind::Empty;

    std::string source_path_;
    std::string image_path_;
    std::string status_;

    std::vector<std::string> lines_;

    std::size_t scroll_line_ = 0;

    bool Fail(const std::string& message) {
        kind_ = PreviewKind::Error;
        status_ = message;
        lines_ = {message};
        return false;
    }

    std::size_t VisibleLineCount() const {
        return height > 44
            ? static_cast<std::size_t>((height - 44) / 16)
            : 1;
    }

    void ClampScroll() {
        const auto count = VisibleLineCount();

        scroll_line_ = lines_.size() > count
            ? std::min(scroll_line_, lines_.size() - count)
            : 0;
    }

    static std::string Lower(std::string value) {
        std::transform(
            value.begin(),
            value.end(),
            value.begin(),
            [](unsigned char c) {
                return static_cast<char>(std::tolower(c));
            });

        return value;
    }

    static bool IsImageExtension(const std::string& e) {
        return e == ".png"  || e == ".jpg"  ||
               e == ".jpeg" || e == ".bmp"  ||
               e == ".gif"  || e == ".webp" ||
               e == ".tif"  || e == ".tiff" ||
               e == ".svg";
    }

    static bool IsTextExtension(const std::string& e) {
        return e == ".txt"  || e == ".md"   ||
               e == ".log"  || e == ".ini"  ||
               e == ".cfg"  || e == ".conf" ||
               e == ".json" || e == ".xml"  ||
               e == ".html" || e == ".css"  ||
               e == ".js"   || e == ".ts"   ||
               e == ".cpp"  || e == ".h"    ||
               e == ".hpp"  || e == ".c"    ||
               e == ".py"   || e == ".sh"   ||
               e == ".yaml" || e == ".yml"  ||
               e == ".csv";
    }

    static bool LooksLikeText(
        const std::vector<unsigned char>& bytes) {

        if (bytes.empty()) {
            return true;
        }

        std::size_t printable = 0;

        for (unsigned char c : bytes) {
            if (c == '\n' || c == '\r' || c == '\t' ||
                (c >= 32 && c < 127) || c >= 0xC2) {
                ++printable;
            }
        }

        return printable * 100 / bytes.size() >= 85;
    }
};
