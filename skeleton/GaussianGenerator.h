#pragma once
#include "ParticleGenerator.h"

class GaussianGenerator : public ParticleGenerator {
public:
    GaussianGenerator(std::string name);
    virtual ~GaussianGenerator();

    virtual std::list<Particle*> generateParticles() override;


    void setPosDeviation(const Vector3& deviation);
    void setVelDeviation(const Vector3& deviation);

protected:
    Vector3 pos_deviation;
    Vector3 vel_deviation;

 
    std::normal_distribution<double> normal_dist;
};