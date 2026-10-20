#pragma once
#include <string>
#include <map>

namespace NexusOS {
namespace Print {

enum class JobStatus {
    QUEUED,
    PRINTING,
    COMPLETED,
    FAILED
};

struct PrintJob {
    int job_id;
    std::string document_name;
    int total_pages;
    JobStatus status;
};

class PrintSpooler {
private:
    std::map<int, PrintJob> print_queue;
    int next_job_id = 1;

public:
    int SubmitJob(const std::string& doc_name, int pages) {
        if (pages <= 0) return -1;
        int id = next_job_id++;
        print_queue[id] = {id, doc_name, pages, JobStatus::QUEUED};
        return id;
    }

    bool CancelJob(int job_id) {
        auto it = print_queue.find(job_id);
        if (it != print_queue.end() && 
            (it->second.status == JobStatus::QUEUED || it->second.status == JobStatus::PRINTING)) {
            print_queue.erase(it);
            return true;
        }
        return false;
    }

    JobStatus GetJobStatus(int job_id) const {
        auto it = print_queue.find(job_id);
        if (it != print_queue.end()) {
            return it->second.status;
        }
        return JobStatus::FAILED;
    }
};

} // namespace Print
} // namespace NexusOS
