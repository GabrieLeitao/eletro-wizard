#ifndef EMFIELDS_H
#define EMFIELDS_H

#include <vector>
#include "vec3.h"  // Assuming you have a Vec3 class

class EMFields {
public:
    EMFields(int Nx, int Ny, int Nz, float spacing);

    void applyPointChargeElectricField(Vec3 charge_position, double charge_magnitude);
    void applyCurrentToMagneticField(Vec3 current_position, double current_strength);
    void updateFields(double dt);
    void printFields(void);

    std::vector<Vec3> electric_field, magnetic_field;
    int Nx, Ny, Nz;
    float spacing;

private:
    int index(int i, int j, int k) const;
};

#endif
