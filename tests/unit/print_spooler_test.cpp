#include "../test_framework.h"
#include "../../core/print/print_spooler.h"

using namespace NexusOS::Print;

TEST(PRN_JobSubmission) {
    PrintSpooler spooler;
    int job_id = spooler.SubmitJob("annual_report.pdf", 12);
    ASSERT_TRUE(job_id > 0);
    ASSERT_EQ(static_cast<int>(JobStatus::QUEUED), static_cast<int>(spooler.GetJobStatus(job_id)));
    
    // Invalid job (0 pages)
    ASSERT_EQ(-1, spooler.SubmitJob("empty.txt", 0));
}

TEST(PRN_JobCancellation) {
    PrintSpooler spooler;
    int job_id = spooler.SubmitJob("notes.txt", 2);
    ASSERT_TRUE(spooler.CancelJob(job_id));
    
    // Status should be FAILED/Not Found after cancellation
    ASSERT_EQ(static_cast<int>(JobStatus::FAILED), static_cast<int>(spooler.GetJobStatus(job_id)));
}

int main() {
    return TestRegistry::RunAll();
}
