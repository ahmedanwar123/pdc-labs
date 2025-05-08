# CUDA Vector Operations

This project demonstrates basic CUDA kernels using:

- CURAND to generate random values
- Element-wise vector addition
- Normalization of 4D vectors (`float4`)

## Requirements

- NVIDIA GPU with CUDA support
- CUDA Toolkit installed (`nvcc`, `curand` libraries)

## Compilation

To compile the CUDA program:

```bash
nvcc -o vector_ops_cuda vector_ops_cuda.cu -lcurand
```
