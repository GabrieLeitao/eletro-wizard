#include "emfield.h"
#include <iostream>
#include <cmath>

const double epsilon_0 = 8.854e-12; // Vacuum permittivity in F/m
const double mu_0 = 4 * M_PI * 1e-7; // Vacuum permeability in H/m

EMFields::EMFields(int Nx, int Ny, int Nz, float spacing)
    : Nx(Nx), Ny(Ny), Nz(Nz), spacing(spacing) {
    electric_field.resize(Nx * Ny * Nz, Vec3(0, 0, 0));
    magnetic_field.resize(Nx * Ny * Nz, Vec3(0, 0, 0));
}

int EMFields::index(int i, int j, int k) const {
    return i + Nx * (j + Ny * k);
}

void EMFields::applyPointChargeElectricField(Vec3 charge_position, double charge_magnitude) {
    // Apply electric field from a point charge
    for (int k = 0; k < Nz; ++k) {
        for (int j = 0; j < Ny; ++j) {
            for (int i = 0; i < Nx; ++i) {
                int idx = index(i, j, k);
                Vec3 r = Vec3(i * spacing, j * spacing, k * spacing) - charge_position;
                double r_magnitude = r.magnitude();
                if (r_magnitude > 0) {
                    double E = charge_magnitude / (4 * M_PI * epsilon_0 * r_magnitude * r_magnitude);
                    electric_field[idx] = r * (E / r_magnitude);
                }
            }
        }
    }
}

void EMFields::applyCurrentToMagneticField(Vec3 current_position, double current_strength) {
    for (int k = 0; k < Nz; ++k) {
        for (int j = 0; j < Ny; ++j) {
            for (int i = 0; i < Nx; ++i) {
                int idx = index(i, j, k);
                magnetic_field[idx] = magnetic_field[idx] + Vec3(0, 0, current_strength);
            }
        }
    }
}

void EMFields::updateFields(double dt) {
    for (int k = 0; k < Nz; ++k) {
        for (int j = 0; j < Ny; ++j) {
            for (int i = 0; i < Nx; ++i) {
                int idx = index(i, j, k);
                electric_field[idx] = electric_field[idx];
                // magnetic_field[idx] = magnetic_field[idx] + electric_field[idx].cross(Vec3(0, 0, 1)) * dt;
            }
        }
    }
}

void EMFields::printFields(void) {
    // Print field values (for debugging)
    for (int k = 0; k < Nz; ++k) {
        for (int j = 0; j < Ny; ++j) {
            for (int i = 0; i < Nx; ++i) {
                int idx = index(i, j, k);
                std::cout << "Point (" << i << ", " << j << ", " << k << ") Electric Field: ";
                electric_field[idx].print();
                std::cout << "Magnetic Field: ";
                magnetic_field[idx].print();
            }
        }
    }
}