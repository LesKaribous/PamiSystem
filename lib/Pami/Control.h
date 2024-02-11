#pragma once

struct Pose {
    float x; // Position en X (mm)
    float y; // Position en Y (mm)
    float rot; // Rotation en degrés

    // Méthodes pour définir les valeurs
    void setX(float _x) { x = _x; }
    void setY(float _y) { y = _y; }
    void setRot(float _rot) { rot = _rot; }

    // Méthodes pour obtenir les valeurs
    float getX() const { return x; }
    float getY() const { return y; }
    float getRot() const { return rot; }
}; 

// Structure pour représenter undéplacement polaire relatif du robot
struct PolarMove {
    float rotation1; // Première rotation pour s'aligner vers la cible
    float distance;  // Déplacement linéaire vers la cible
    float rotation2; // Rotation finale pour ajuster l'orientation
};



