#pragma once
// Pointer to a temporary for the duration of the full-expression, for APIs that take
// `const T*` (e.g. ID3DXSprite::Draw(..., rvalue_ptr(D3DXVECTOR3(0, 0, 0)), ...)). Taking
// the address of a temporary directly is an MSVC extension; this is the portable equivalent.
template <typename T>
inline const T* rvalue_ptr(const T& value) { return &value; }
