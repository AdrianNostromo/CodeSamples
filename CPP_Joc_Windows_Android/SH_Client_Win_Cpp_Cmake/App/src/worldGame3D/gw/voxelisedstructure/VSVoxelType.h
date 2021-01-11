#pragma once

class VSVoxelType {
public:
    static const int Empty = 0;
    static const int Wall = 1;
    static const int ElectricCable_Zero = 2;
    static const int ElectricCable_Positive = 3;
    static const int SolarCellMaterial_Idle = 4;
    static const int SolarCellMaterial_Charged = 5;
    static const int IonEngineMaterial_Zero = 6;
    static const int IonEngineMaterial_Active = 7;

};
