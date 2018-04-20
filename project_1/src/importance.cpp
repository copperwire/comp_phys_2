#include <armadillo>
#include "../include/vmc.h"

using namespace arma;

const double PI = 3.141592653589793;
double Importance::metropolis_hastings(WaveFunc *psi_t, double prev_E_l){
    mat R_p(size(R));
    R_p = R;
	double dt = 0.001; // dt in [0.001,0.01] should produce stable ground state results.
	double beta = psi_t -> params[2];

    uniform_int_distribution<int> dis_r(0, N_p - 1);
    uniform_real_distribution<double> dis_step(-1.0, 1.0);
    uniform_real_distribution<double> dis_p(0.0, 1.0);
	normal_distribution<double> dis_zeta(0.0, 1.0);

	// Pick particle j to move.
    int j = dis_r(*gen);

	// Scale z-position of moved particle with beta
	if(N_d == 3){
		R_p(j,2) *= beta;
	}

	// Calculate drift force for that particle.
    mat F_drift(1, N_d);
	F_drift = psi_t -> drift_force(R.row(j));

	// Move the particle
	double zeta = dis_zeta(*gen);
	R_p.row(j) += 0.5*F_drift*dt + zeta*sqrt(dt);
	
	
    double P = psi_t -> ratio(R, R_p, j);

	// Calculate drift force for particle j in proposed position
	mat F_drift_proposed(1, N_d);
	F_drift_proposed = psi_t -> drift_force(R_p.row(j));

    mat term1(1, N_d);
    mat term2(1, N_d);
    double term3;

    double Green_prev = 0.0;
    double Green_proposed = 0.0;

	// Calculate greens functions
	for(int i = 0; i < N_d; i++){
		term1 = R_p(j,i) - R(j,i) - 0.5*dt*F_drift(i);
		term2 = R(j,i) - R_p(j,i) - 0.5*dt*F_drift_proposed(i);
		term3 = 2*dt;

		Green_prev += accu(-(term1*term1)/term3);
		Green_proposed += accu(-(term2*term2)/term3);
	}
	// Scale by term 4 after loop to save some flops
	double term4 = 1/pow((2*PI*dt),(3*N_p/2));
	Green_prev *= term4;
	Green_proposed *= term4;

	double q = P*Green_prev/Green_proposed;
    double eps = dis_p(*gen);
    if(eps < q){
        R = R_p;
        psi_t -> update();
        return psi_t -> E_l(R);
    }
    else{
        return prev_E_l;
    }
}
