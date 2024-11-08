// #include <NTL/ZZ.h>
// #include <NTL/vector.h>
// #include <iostream>
// #include <vector>
//
// using namespace NTL;
// using namespace std;;;;;;
//
// // Function to generate shares from a secret using Shamir's Secret Sharing
// void generate_shares(vector<pair<long, ZZ>>& shares, int n, int k, const ZZ& secret, const ZZ& prime) {
//     Vec<ZZ> coeffs;
//     coeffs.SetLength(k);
//     coeffs[0] = secret;  // Constant term is the secret
//
//     // Generate random coefficients for the polynomial (mod prime)
//     for (int i = 1; i < k; ++i) {
//         coeffs[i] = RandomBnd(prime);  // Random coefficient in range [0, prime-1]
//     }
//
//     // Generate shares by evaluating polynomial at x = 1, 2, ..., n
//     for (int i = 1; i <= n; ++i) {
//         ZZ y = coeffs[0];
//         ZZ x = to_ZZ(i);
//
//         for (int j = 1; j < k; ++j) {
//             y += coeffs[j] * power(x, j);  // Evaluate polynomial
//             y %= prime;
//         }
//         shares.push_back({i, y});
//     }
// }
//
// // Function to reconstruct the secret from k shares using Lagrange interpolation
// ZZ reconstruct_secret(const vector<pair<long, ZZ>>& shares, int k, const ZZ& prime) {
//     ZZ secret = ZZ(0);
//
//     for (int i = 0; i < k; ++i) {
//         ZZ term = shares[i].second;
//         ZZ xi = to_ZZ(shares[i].first);
//
//         for (int j = 0; j < k; ++j) {
//             if (i != j) {
//                 ZZ xj = to_ZZ(shares[j].first);
//                 ZZ num = -xj;
//                 ZZ denom = xi - xj;
//
//                 num %= prime;
//                 denom %= prime;
//
//                 // Calculate modular inverse of denom
//                 ZZ denom_inv;
//                 InvMod(denom_inv, denom, prime);
//
//                 // Update term: term *= (xi - xj)^-1 * (-xj)
//                 term = (term * num % prime) * denom_inv % prime;
//             }
//         }
//         secret = (secret + term) % prime;
//     }
//
//     if (secret < 0) {
//         secret += prime;  // Ensure non-negative result
//     }
//     return secret;
// }
//
// int main() {
//     int n = 5;  // Total number of shares to generate
//     int k = 3;  // Minimum number of shares needed to reconstruct the secret
//
//     ZZ secret = to_ZZ(12);  // Secret value to be shared
//     ZZ prime = to_ZZ(7919);   // A large prime for finite field
//
//     // Generate shares
//     vector<pair<long, ZZ>> shares;
//     generate_shares(shares, n, k, secret, prime);
//
//     // Display shares
//     cout << "Generated Shares:" << endl;
//     for (auto& share : shares) {
//         cout << "Share (" << share.first << ", " << share.second << ")" << endl;
//     }
//
//     // Reconstruct the secret using any k shares
//     vector<pair<long, ZZ>> selected_shares(shares.begin(), shares.begin() + k);
//     ZZ reconstructed_secret = reconstruct_secret(selected_shares, k, prime);
//
//     // Display the reconstructed secret
//     cout << "Reconstructed Secret: " << reconstructed_secret << endl;
//
//     return 0;
// }
// //
// // Created by kiarash on 11/7/24.
// //
