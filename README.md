# RAII to STL-Compatible Container Workshop  
*A two-day workshop in Modern C++ and Best Practices at Uppsala University, Campus Gotland*

The workshop guides participants through building a small RAII-based resource manager and evolving it into a container that plays nicely with the C++ standard library.

The code in this repository represents **one possible outcome** of the workshop. It is meant as a **reference implementation** for students after completing the exercises.  
It is **not** production-ready code, nor is it intended as a drop-in replacement for `std::vector` or any other standard container.

The purpose is educational: to demonstrate idiomatic RAII, move semantics, exception safety, standard-library conventions, and how to design types that integrate cleanly with generic algorithms.

---

## Workshop Overview

The workshop spans two days:

### Day 1: RAII and the Rule of Five

Participants implement a minimal but correct resource manager, focusing on:

- Setting up a C++ project with strict code-quality enforcement.
- Implementing exception-safe resource-handling classes (Rule of Five).
- Understanding move semantics and rvalue references in practice.
- Designing types with clear invariants protected by a public interface
- Ensuring at least the Basic exception guarantee.
- Using the debugger to step through operations and inspect object state.

This forms the foundation for building user-defined types that behave predictably under construction, destruction, copying, and moving.

---

### Day 2: The Design of the STL

Building on the day one RAII type, participants extend it into a small container that follows standard-library conventions:

- Navigating cppreference.com to understand type requirements and contracts.
- - Making the type **regular** or **semi-regular**.
- Describing how the STL separates **data structures** from **algorithms**.
- Adding iterator support (including type aliases) so the type works with:
  - range-for loops  
  - `std::find`, `std::sort`, `std::accumulate`, `std::all_of`, etc.
- Using lambda expressions as algorithm predicates.
- Using a debugger to verify iterator behaviour and container state.

The end result is a small educational container that demonstrates the mechanics behind STL compatibility. The code sacrifices completeness for clarity, leaving space for students to experiment, extend, and improve.
