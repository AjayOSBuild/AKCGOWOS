# ADR 0007: Git Workflow and Strategy

## Status
Accepted

## Context
A disciplined version control strategy is essential to prevent regressions and maintain a clear history of architectural decisions and feature implementations. Git is the source of truth for this project.

## Decision

We will strictly adhere to the following Git workflow:

### 1. Branching Strategy
*   `main`: The production/release branch. Never work directly on `main` for experimental development. Never automatically push to this branch.
*   `development`: The primary integration branch where features are merged after passing QEMU testing.
*   `feature/<TASK-ID>-<description>`: Isolated branches for individual tasks (e.g., `feature/BOOT-001-uefi-test`).

### 2. Task Requirements
Every completed task merged into `development` must produce:
1.  Implementation code.
2.  Automated tests (Smoke, Unit, or Integration).
3.  Documentation updates (if architecture, API, or build processes changed).
4.  A formatted Git commit.

### 3. Commit Message Format
Commit messages must lead with the Task ID to ensure traceability back to the project roadmap and task database.
**Format:** `<TASK-ID>: <description>`
**Example:** `git commit -m "BOOT-001: create UEFI QEMU boot environment"`

## Consequences
*   **Positive:** Ensures that the `main` and `development` branches remain stable and that every change is easily traceable to a specific roadmap requirement.
*   **Negative:** Enforces a slight overhead in branch management for very small changes, but this is necessary for automated AI agent tracking.
