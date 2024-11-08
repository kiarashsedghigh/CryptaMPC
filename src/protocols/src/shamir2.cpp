// #include <flint/fmpz.h>
// #include <flint/fmpz_poly.h>
// #include <iostream>
//
// int main() {
//     // Initialize polynomial object
//     fmpz_poly_t poly;
//     fmpz_poly_init(poly);  // Initialize the polynomial
//
//     // Define the degree of the polynomial
//     long degree = 3;
//
//     // Set the coefficients (using fmpz_t)
//     fmpz_t coeff;
//     fmpz_init(coeff);
//
//     // Assign values to coefficients (coefficients are of type fmpz_t)
//     fmpz_set_ui(coeff, 5);  // Set coefficient for x^0 (constant term)
//     fmpz_poly_set_coeff_fmpz(poly, 0, coeff);  // Set poly[0] = 5
//
//     fmpz_set_ui(coeff, 3);  // Set coefficient for x^1
//     fmpz_poly_set_coeff_fmpz(poly, 1, coeff);  // Set poly[1] = 3
//
//     fmpz_set_ui(coeff, 7);  // Set coefficient for x^2
//     fmpz_poly_set_coeff_fmpz(poly, 2, coeff);  // Set poly[2] = 7
//
//     fmpz_set_ui(coeff, 1);  // Set coefficient for x^3
//     fmpz_poly_set_coeff_fmpz(poly, 3, coeff);  // Set poly[3] = 1
//
//     // Print the polynomial
//     std::cout << "Polynomial: ";
//     fmpz_poly_print(poly);
//     std::cout << std::endl;
//
//     // Clear allocated memory
//     fmpz_clear(coeff);
//     fmpz_poly_clear(poly);
//
//     return 0;
// }
