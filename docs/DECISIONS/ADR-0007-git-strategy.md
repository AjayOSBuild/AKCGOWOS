# ADR 0007: Git Workflow and Strategy

## Status
Accepted (Amended)

## Context
A disciplined version control strategy is essential to prevent regressions. However, to accelerate early prototyping, the branching strategy has been temporarily streamlined.

## Decision
*   **Direct Commits:** All experimental and feature development will be committed and pushed directly to the `main` branch.
*   **Duration:** This override remains in effect until the completion of Phase 19 (Installer). 
*   **Post-Phase 19:** The project will revert to standard isolated `feature/*` branching to protect stability during final QA and Hardware Validation.

## Consequences
*   **Positive:** Faster development loop and reduced Git management overhead during the foundational phases.
*   **Negative:** Increased risk of breaking the main tree if untested code is committed.
