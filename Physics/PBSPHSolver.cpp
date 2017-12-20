#include "PBSPHSolver.h"
#include "PBSPHParticle.h"

#include "IndexedFinder.h"
#include "PBSPHBoundarySolver.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

void PBSPHSolver::simulate(const float dt, const float effectLength, const float searchLength, const int maxIter)
{
	for (auto p : particles) {
		p->init();
	}

	//static int i;
	//i++;
	//const auto bb = Box3d( this->boundary.getMin(), this->boundary.getMax() + Vector3dd( std::sin(i * 0.1f ) * 20.0, 0.0, 0.0) );
	PBSPHBoundarySolver boundarySolver(boundary);
	for (auto p : particles) {
		p->addExternalForce(externalForce);
		p->predictPosition_(dt);
	}

	IndexedFinder finder(searchLength);
	for (auto p : particles) {
		finder.add(p);
	}
	finder.createPairs();
	const auto& pairs = finder.getPairs();

	SPHKernel kernel(effectLength);
	for (auto p : particles) {
		p->setKernel(&kernel);
	}

	for (int iter = 0; iter < maxIter; ++iter) {
		for (int i = 0; i < particles.size(); ++i) {
			const auto p = static_cast<PBSPHParticle*>(particles[i]);
			p->setDensity(0.0f);
			p->dx = Math::Vector3df(0,0,0);
		}

		boundarySolver.addDX(particles, dt);

		for (int i = 0; i < particles.size(); ++i) {
			const auto p = static_cast<PBSPHParticle*>(particles[i]);
			p->addSelfDensity();
		}

#pragma omp parallel for
		for (int i = 0; i < pairs.size(); ++i) {
			const auto p1 = static_cast<PBSPHParticle*>( pairs[i].getParticle1() );
			const auto p2 = static_cast<PBSPHParticle*>( pairs[i].getParticle2() );
			p1->addDensity(*p2);
			p2->addDensity(*p1);
		}

#pragma omp parallel for
		for (int i = 0; i < pairs.size(); ++i) {
			const auto p1 = static_cast<PBSPHParticle*>(pairs[i].getParticle1());
			const auto p2 = static_cast<PBSPHParticle*>(pairs[i].getParticle2());
			p1->calculatePressure(*p2);
			p2->calculatePressure(*p1);
		}

		boundarySolver.calculatePressure(particles);

		for (int i = 0; i < particles.size(); ++i) {
			const auto p = particles[i];
			p->updatePredictPosition();
		}
	}

	for (int i = 0; i < particles.size(); ++i) {
		particles[i]->xvisc = Vector3df(0, 0, 0);
	}
	boundarySolver.calculateViscosity(particles);
	for (int i = 0; i < pairs.size(); ++i) {
		const auto p1 = static_cast<PBSPHParticle*>(pairs[i].getParticle1());
		const auto p2 = static_cast<PBSPHParticle*>(pairs[i].getParticle2());
		p1->calculateViscosity(*p2);
		p2->calculateViscosity(*p1);
	}

	for (auto p : particles) {
		p->updateVelocity(dt);
		p->addVelocity(p->xvisc);
		p->updatePosition();
		//	p->integrate(dt);
	}

}