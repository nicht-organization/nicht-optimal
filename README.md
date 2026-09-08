# nicht-optimal

Zero-allocation, SIMD-friendly meta-heuristic solver engine and graph metric database for hard optimization problems (TSP, graph partitioning, and spectral metric indexing) written in C.

Part of the **nicht-organization** ecosystem.

---

## Features

* **Zero-Allocation Core:** Operates over flat contiguous memory buffers (`alignas(64)` tapes) without dynamic memory allocation inside solver loops.
* **Multi-Algorithm Pipeline:**
  * Zero-alloc TSPLIB buffer parser (`tsplib_parser.h`).
  * In-place 2-Opt SIMD local search solver (`solvers.h`).
  * Nearest-Neighbor (NN) constructive greedy search.
  * Hybrid NN + 2-Opt acceleration engine.
* **Cross-Language Integration Pathways:**
  * Clean C ABI headers (`optimal.h`) designed for FFI bindings in **Python** (`ctypes`/`cffi`), **Rust** (`cxx`/`bindgen`), and **Go** (`cgo`).
* **Vendor Integration:** Seamlessly interfaces with `nicht-collections` (`phi_heap.h`, `spectral_index.h`) via Git submodules.

---

## Repository Architecture

```text
nicht-optimal/
├── include/
│   └── nicht/
│       ├── tsplib_parser.h   # Raw memory TSPLIB parser & matrix synthesizer
│       ├── solvers.h         # 2-Opt local search & SIMD edge swap routines
│       └── optimal.h         # Master umbrella header & submodule resolver
├── vendor/
│   └── nicht-collections/    # Git Submodule (phi_heap, spectral_index)
├── tests/
│   └── test_parser.c         # Unit test suite for TSPLIB matrix validation
└── benchmarks/
    ├── bench_solvers.c       # 100-node baseline solver benchmark
    └── bench_multi_solvers.c # Big data multi-algorithm benchmark harness
```

## Performance Benchmark
Results measured on standard Linux x86_64 / GCC -O3:

| Graph Size | Algorithm | Final Tour Length | Iterations | Execution Time |
| :--- | :--- | :--- | :--- | :--- |
| 100 Nodes | Pure 2-Opt | 8135.09 | 398 | 1.360 ms |
| 100 Nodes | Nearest Neighbor | 9345.61 | 100 | 0.012 ms |
| 100 Nodes | Hybrid NN + 2-Opt | 8053.74 | 131 | 0.186 ms |
| 300 Nodes | Hybrid NN + 2-Opt | 13852.09 | 400 | 4.732 ms |
| 500 Nodes | Hybrid NN + 2-Opt | 17261.18 | 658 | 22.489 ms |

---

## Quickstart

1. Clone repository with submodules:
   ```bash
   git clone --recursive https://github.com/nicht-organization/nicht-optimal.git
   cd nicht-optimal
   ```

2. Run full clean build, unit tests, and multi-solver benchmarks:
   ```bash
   chmod +x clean_build.sh
   ./clean_build.sh
   ```

---

## Language Library Bindings (FFI Path)

nicht-optimal exposes pure C functions with static linkability for direct consumption across languages:

* Python: Connect via ctypes.CDLL("./libnicht_optimal.so")
* Rust: Bind via bindgen or cc-rs in build.rs
* Go: Import directly using cgo (#cgo CFLAGS: -Iinclude)

---

## License

[The Unlicense][LICENSE] - Free and unencumbered public domain software.