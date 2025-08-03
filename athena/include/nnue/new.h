
// class Accumulator {

// };

// class AccumulatorStack {
// private:
// 	std::vector<Accumulator> accumulators;
//   std::size;

// public:
//   AccumulatorStack();

//   void reset() {
//     size = 1;
//   }

//   void push() {
//     size++;
//   }

//   void pop() {
//     size--;
//   }

//   void forward_increamental_update() {

//   }

//   void backward_increamental_update() {

//   }

// };

// // network.h

// class Network {

// };

// // nnue.h

// class NNUE {

// };

// #pragma once

// #include <array>
// #include <cstddef>
// #include <cstring>
// #include <memory>

// #ifdef USE_AVX2
// #include <immintrin.h>
// #endif

// namespace athena
// {

// // static constexpr int INPUT_SIZE = 4 * 4 * 160 * 6;
// // static constexpr int HIDDEN_SIZE = 128;

// // // template<std::size_t N>
// // // class Vector
// // // {
// // //     private:

// // //         std::array<float, N> elements;

// // //     public:

// // //         // Default constructor
// // //         Vector() = default;

// // //         // Default destructor
// // //         ~Vector() = default;

// // //         // Copy constructor
// // //         Vector(const Vector& other) = default;

// // //         // Move constructor
// // //         Vector(Vector&& other) noexcept = default;

// // //         // Copy assignment
// // //         Vector& operator=(const Vector& other) = default;

// // //         // Move assignment
// // //         Vector& operator=(Vector&& other) noexcept = default;

// // //         // Access element
// // //         float& operator[](std::size_t i) { return elements[i]; }

// // //         // Access element
// // //         const float& operator[](std::size_t i) const { return elements[i]; }

// // //         // Load data from raw array
// // //         void load(const float* data) {
// // //             std::memcpy(elements.data(), data, sizeof(float) * N);
// // //         }

// // //         // Get pointer to raw data
// // //         float* data() { return elements.data(); }

// // //         // Get pointer to raw data
// // //         const float* data() const { return elements.data(); }

// // //         // Dot product
// // //         float dot(const Vector<N>& other) const;

// // //         // Vector addition
// // //         Vector<N> operator+(const Vector<N>& vec) const;
// // // };

// // // template<std::size_t N>
// // // float Vector<N>::dot(const Vector<N>& other) const
// // // {
// // //     static_assert(N % 8 == 0, "Vector size must be multiple of 8; no tail handling implemented.");

// // //     float sum = 0.0f;

// // // #ifdef USE_AVX2
// // //     __m256 sum256 = _mm256_setzero_ps();
// // //     for (size_t i = 0; i < N; i += 8)
// // //     {
// // //         __m256 va = _mm256_loadu_ps(&elements[i]);
// // //         __m256 vb = _mm256_loadu_ps(&other.elements[i]);
// // //         sum256 = _mm256_fmadd_ps(va, vb, sum256);
// // //     }
// // //     __m128 hi = _mm256_extractf128_ps(sum256, 1);
// // //     __m128 lo = _mm256_castps256_ps128(sum256);
// // //     __m128 sum128 = _mm_add_ps(lo, hi);
// // //     sum128 = _mm_hadd_ps(sum128, sum128);
// // //     sum128 = _mm_hadd_ps(sum128, sum128);
// // //     _mm_store_ss(&sum, sum128);
// // // #else
// // //     for (std::size_t i = 0; i < N; ++i)
// // //         sum += elements[i] * other.elements[i];
// // // #endif

// // //     return sum;
// // // }

// // // template<std::size_t N>
// // // Vector<N> Vector<N>::operator+(const Vector<N>& vec) const
// // // {
// // //     static_assert(N % 8 == 0, "Vector size must be multiple of 8; no tail handling implemented.");

// // //     Vector<N> result;

// // // #ifdef USE_AVX2
// // //     for (std::size_t i = 0; i < N; i += 8)
// // //     {
// // //         __m256 a = _mm256_loadu_ps(elements.data() + i);
// // //         __m256 b = _mm256_loadu_ps(vec.elements.data() + i);
// // //         __m256 sum = _mm256_add_ps(a, b);
// // //         _mm256_storeu_ps(result.elements.data() + i, sum);
// // //     }
// // // #else
// // //     for (std::size_t i = 0; i < N; ++i)
// // //         result[i] = elements[i] + vec.elements[i];
// // // #endif

// // //     return result;
// // // }

// // // template<std::size_t M, std::size_t N>
// // // class Matrix
// // // {
// // //     private:

// // //     public:
// // // std::array<float, M * N> elements;
// // //         // Default constructor
// // //         Matrix() = default;

// // //         // Copy constructor
// // //         Matrix(const Matrix& other) = default;

// // //         // Move constructor
// // //         Matrix(Matrix&& other) noexcept = default;

// // //         // Copy assignment
// // //         Matrix& operator=(const Matrix& other) = default;

// // //         // Move assignment
// // //         Matrix& operator=(Matrix&& other) noexcept = default;

// // //         // Access element at row i, column j
// // //         float& operator()(std::size_t i, std::size_t j) {
// // //             return elements[i * N + j];
// // //         }

// // //         // Access element at row i, column j
// // //         const float& operator()(std::size_t i, std::size_t j) const {
// // //             return elements[i * N + j];
// // //         }

// //         // Vector<M> multiply(const Vector<N>& vec) const
// //         // {
// //         //     Vector<M> result;

// //         // #ifdef USE_AVX2
// //         //     for (std::size_t r = 0; r < M; ++r)
// //         //     {
// //         //         __m256 sum = _mm256_setzero_ps();
// //         //         for (std::size_t c = 0; c < N; c += 8)
// //         //         {
// //         //             __m256 row_vec = _mm256_loadu_ps(&elements[r][c]);
// //         //             __m256 vec_val = _mm256_loadu_ps(&vec.elements[c]);
// //         //             sum = _mm256_fmadd_ps(row_vec, vec_val, sum);
// //         //         }

// //         //         // Horizontal sum of the 8 floats in sum without storing to memory:
// //         //         __m128 low  = _mm256_castps256_ps128(sum);           // low 128 bits
// //         //         __m128 high = _mm256_extractf128_ps(sum, 1);         // high 128 bits
// //         //         __m128 sum128 = _mm_add_ps(low, high);
// //         //         sum128 = _mm_hadd_ps(sum128, sum128);
// //         //         sum128 = _mm_hadd_ps(sum128, sum128);

// //         //         float s = _mm_cvtss_f32(sum128);

// //         //         result.elements[r] = s;
// //         //     }
// //         // #else
// //         //     for (std::size_t r = 0; r < Rows; ++r)
// //         //     {
// //         //         float s = 0.0f;
// //         //         for (std::size_t c = 0; c < Cols; ++c)
// //         //             s += elements[r][c] * vec.elements[c];
// //         //         result.elements[r] = s;
// //         //     }
// //         // #endif

// //         //     return result;
// //         // }
// // // };

// // // template<std::size_t N>
// // // class Vector
// // // {
// // //     private:

// // //         std::unique_ptr<float[]> elements;
// // // };

// // // template<std::size_t M, std::size_t N>
// // // class Matrix
// // // {
// // //     private:

// // //         std::unique_ptr<float[]> elements;
// // // };

// // template<typename T>
// // class AlignedArray
// // {

// // };

// // // Cache line size for alignment
// // constexpr std::size_t CACHE_LINE_SIZE = 64;

// // template<std::size_t N>
// // class Vector
// // {
// //     private:

// //         float* elements;
// // };

// // template<std::size_t M, std::size_t N>
// // class Matrix
// // {
// //     private:

// //         float* elements;
// // };

// // template<std::size_t N>
// // Vector<N> add(const Vector<N>& vec1, const Vector<N>& vec2)
// // {

// // };

// // template<std::size_t N>
// // float dot(const Vector<N>& vec1, const Vector<N>& vec2)
// // {

// // };

// // template<std::size_t M, std::size_t N>
// // Vector<M> multiply(const Matrix<M, N>& mat, const Vector<N>& vec)
// // {

// // }

// // template<std::size_t M, std::size_t N>
// // class Dense
// // {
// //     private:

// //         Matrix<M, N> weights;
// //         Vector<N> biases;

// //         // std::unique_ptr<float[]> weights = nullptr;
// //         // std::unique_ptr<float[]> biases  = nullptr;

// //     public:

// //         // void load(std::unique_ptr<float[]> w, std::unique_ptr<float[]> b)
// //         // {
// //         //     weights = std::move(w);
// //         //     biases  = std::move(b);
// //         // }

// //         void forward(const float* input, float* output) const
// //         {
// //             static_assert(InDim % 8 == 0, "InDim must be multiple of 8 for AVX2");

// //             for (std::size_t i = 0; i < OutDim; ++i)
// //             {
// //                 float sum = 0.0f;

// //             #ifdef USE_AVX2
// //                 __m256 sum256 = _mm256_setzero_ps();
// //                 const float* w_row = &weights[i * InDim];

// //                 for (std::size_t i = 0; i < InDim; i += 8) {
// //                     __m256 va = _mm256_loadu_ps(&w_row[i]);
// //                     __m256 vb = _mm256_loadu_ps(&input[i]);
// //                     sum256 = _mm256_fmadd_ps(va, vb, sum256);
// //                 }

//                 // __m128 hi = _mm256_extractf128_ps(sum256, 1);
//                 // __m128 lo = _mm256_castps256_ps128(sum256);
//                 // __m128 sum128 = _mm_add_ps(lo, hi);
//                 // sum128 = _mm_hadd_ps(sum128, sum128);
//                 // sum128 = _mm_hadd_ps(sum128, sum128);
//                 // _mm_store_ss(&sum, sum128);
// //             #else
// //                 const float* w_row = &weights[i * InDim];
// //                 for (std::size_t i = 0; i < InDim; ++i)
// //                     sum += w_row[i] * input[i];
// //             #endif

// //                 output[i] = sum + bias[i];
// //             }
// //         }

// //             // Alternative: Block-based computation for very large matrices
// //         void forward_blocked(const float* input, float* output) const
// //         {
// //             constexpr std::size_t BLOCK_SIZE = 64; // Tune based on cache size

// //             // Initialize output with biases
// //             for (std::size_t i = 0; i < OutDim; ++i) {
// //                 output[i] = biases[i];
// //             }

// //             // Block the computation for better cache utilization
// //             for (std::size_t out_block = 0; out_block < OutDim; out_block += BLOCK_SIZE)
// //             for (std::size_t  in_block = 0;  in_block <  InDim;  in_block += BLOCK_SIZE)
// //             {
// //                 std::size_t out_end = out_block + BLOCK_SIZE;
// //                 std::size_t  in_end =  in_block + BLOCK_SIZE;

// //                 // Process the block
// //                 for (std::size_t i = out_block; i < out_end; i += 8)
// //                 for (std::size_t k =  in_block; k <  in_end; k += 8)
// //                 {
// //                     #ifdef USE_AVX2
// //                         __m256 input_vec = _mm256_load_ps(&input[k]);

// //                         for (std::size_t j = 0; j < 8 && (i + j) < out_end; ++j)
// //                         {
// //                             __m256 weight_vec = _mm256_load_ps(&weights[(i + j) * InDim + k]);
// //                             __m256 result = _mm256_mul_ps(weight_vec, input_vec);

// //                             // Horizontal sum
// //                             __m128 hi = _mm256_extractf128_ps(result, 1);
// //                             __m128 lo = _mm256_castps256_ps128(result);
// //                             __m128 sum128 = _mm_add_ps(lo, hi);
// //                             sum128 = _mm_hadd_ps(sum128, sum128);
// //                             sum128 = _mm_hadd_ps(sum128, sum128);

// //                             float partial_sum;
// //                             _mm_store_ss(&partial_sum, sum128);
// //                             output[i + j] += partial_sum;
// //                         }
// //                     #else
// //                         for (std::size_t j = 0; j < 8 && (i + j) < out_end; ++j) {
// //                             const float* w_row = &weights[(i + j) * InDim + k];
// //                             for (std::size_t l = 0; l < 8 && (k + l) < in_end; ++l) {
// //                                 output[i + j] += w_row[l] * input[k + l];
// //                             }
// //                         }
// //                     #endif
// //                 }
// //             }
// //         }
// // };

// // template<std::size_t n0, std::size_t n1, std::size_t n2>
// // class NNUE
// // {
// //     private:

// //         FC<n0, n1> fc1;
// //         FC<n1, n2> fc2;

// //         // Pre-allocated buffers to avoid dynamic allocation
// //         mutable std::unique_ptr<float[]> z1;

// //         // For incremental updates
// //         mutable std::unique_ptr<float[]> z1_cached;
// //         mutable bool cache_valid;

// //     public:

// //         float forward(const float* input)
// //         {
// //             // First layer
// //             fc1.forward(input, z1.get());

// //             // Activation
// //             SCReLU<n1>::forward(z1.get(), a1.get());
// //         }

// //         float incremental_update(std::size_t feature_idx, float old_val, float new_val)
// //         {

// //             const float delta = new_val - old_val;

// //             // Update first layer output incrementally
// //             fc1.incremental_update(feature_idx, delta, z1_cached.get())
// //         }

// //         // float forward(const std::vector<float>& a0)
// //         // {
// //         //     auto z1 = fc1.foward(a0);
// //         //     auto a1 = SCReLU(z1);
// //         //     auto z2 = fc2.foward(a1);
// //         //     return z2[0]; // clamp
// //         // }
// // };

// class NNUE {

// };

// } // namespace athena