# AI Development Context and Rules

This project is co-developed by autonomous AI agents. To prevent system damage, context exhaustion, and architectural drift, all AI models must strictly adhere to the following rules.

## 1. Absolute Directives
*   **Never generate the entire OS in one response.** The project MUST be divided into small, independently testable tasks.
*   **Never modify the host machine's partition table or format a physical disk.** All destructive operations must target QEMU virtual disks.
*   **Never claim success without testing.** Every significant kernel/system-level change must be verified in QEMU before being considered complete.

## 2. Task Sizing & Modularity
*   Tasks must be small enough to complete in 5–30 minutes of work.
*   *Bad Task:* "Build the filesystem."[cite: 1]
*   *Good Task:* "Create the filesystem abstraction."[cite: 1]
*   Large tasks must be automatically split into sub-tasks.

## 3. Escalation Strategy
The local model (Qwen3-14B) is the primary worker. If a task fails, the following escalation path applies[cite: 1]:
1.  Attempt 1: Qwen3-14B[cite: 1].
2.  Attempt 2: Qwen3-14B with compiler/test logs[cite: 1].
3.  Attempt 3: Qwen3-14B with previous attempts[cite: 1].
4.  Attempt 4: Cheap cloud model[cite: 1].
5.  Attempt 5: Expert cloud model / Claude Code (or Gemini)[cite: 1].
6.  Attempt 6: Human review[cite: 1].

## 4. Token Optimization
AI agents must not read the entire Git history or full repository context unless absolutely necessary. Before generating code, agents must[cite: 1]:
1.  Identify the exact task and required symbols.
2.  Retrieve only relevant files and test logs.
3.  Summarize previous failures and send minimal context.

## 5. Tool Permissions
*   **Allowed Automatically:** READ_FILE, WRITE_FILE, GIT_STATUS, GIT_DIFF, COMMIT, RUN_BUILD, RUN_TEST, RUN_QEMU, READ_LOG[cite: 1].
*   **Requires Human Approval:** DELETE_REPOSITORY, FORMAT_DISK, MODIFY_HOST_BOOT, CHANGE_SECURITY_POLICY[cite: 1].
