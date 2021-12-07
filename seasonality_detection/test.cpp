#include <iostream>
#include <cassert>
#include <cmath>

#include "test.hpp"

//fonction de test du TD3 :

void test_intracluster_variance()
{
	// tolerance for comparison of doubles
	const double eps = 0.0001;

	// dimension used for tests
	point::set_dim(1);

	// temporary container
	point p;

	std::cout << "Testing function intracluster_variance()...";

	// test case 1
	const double dist_onepoint_zerodist = 0.0;
	cloud onepoint_zerodist(1, 1, 1);
	p.coords[0] = 0.0;
	onepoint_zerodist.add_point(p, 0);
	assert(("[NOK]", std::fabs(onepoint_zerodist.intracluster_variance() - dist_onepoint_zerodist) < eps));

	// test case 2
	const double dist_onepoint_posdist = 0.25;
	cloud onepoint_posdist(1, 1, 1);
	p.coords[0] = 0.5;
	onepoint_posdist.add_point(p, 0);
	assert(("[NOK]", std::fabs(onepoint_posdist.intracluster_variance() - dist_onepoint_posdist) < eps));

	// test case 3
	const double dist_twopoints = 0.625;
	cloud twopoints(1, 2, 1);
	p.coords[0] = -1.0;
	twopoints.add_point(p, 0);
	p.coords[0] = 0.5;
	twopoints.add_point(p, 0);
	p.coords[0] = -0.5;
	twopoints.set_center(p, 0);
	assert(("[NOK]", std::fabs(twopoints.intracluster_variance() - dist_twopoints) < eps));

	// test case 4
	const double dist_twoclusters = 6.8125;
	cloud twoclusters(1, 4, 2);
	p.coords[0] = -1.0;
	twoclusters.add_point(p, 0);
	p.coords[0] = 0.5;
	twoclusters.add_point(p, 0);
	p.coords[0] = -0.5;
	twoclusters.set_center(p, 0);
	p.coords[0] = -2.0;
	twoclusters.add_point(p, 1);
	p.coords[0] = 2.0;
	twoclusters.add_point(p, 1);
	p.coords[0] = -3.0;
	twoclusters.set_center(p, 1);
	assert(("[NOK]", std::fabs(twoclusters.intracluster_variance() - dist_twoclusters) < eps));

	std::cout << "\t[OK]" << std::endl;
}

void test_init_forgy()
{
	// number of random experiments
	const int K = 10000;
	// tolerance in probability
	const double delta = 0.0625;

	// dimenstion used for tests
	point::set_dim(1);

	// temporary container
	point p;

	std::cout << "Testing function init_forgy()...";

	const double prob_threepoints = 0.3333;
	cloud threepoints(1, 3, 1);
	p.coords[0] = 0.0;
	threepoints.add_point(p, 0);
	p.coords[0] = 1.0;
	threepoints.add_point(p, 0);
	p.coords[0] = 2.0;
	threepoints.add_point(p, 0);
	int cnt = 0;
	for (int k = 0; k < K; k++)
	{
		threepoints.init_forgy();
		if (threepoints.get_center(0).coords[0] == 1.0)
			cnt++;
	}
	assert(("[NOK]", std::fabs(cnt / (double)K - prob_threepoints) < delta));

	std::cout << "\t\t[OK]" << std::endl;
}

void test_init_plusplus()
{
	// number of random experiments
	const int K = 10000;
	// tolerance in probability
	const double delta = 0.0625;

	// dimenstion used for tests
	point::set_dim(1);

	// temporary container
	point p;

	std::cout << "Testing function init_plusplus()...";

	// test case 1
	const double prob_threepoints = 0.3333;
	cloud threepoints(1, 3, 1);
	p.coords[0] = 0.0;
	threepoints.add_point(p, 0);
	p.coords[0] = 1.0;
	threepoints.add_point(p, 0);
	p.coords[0] = 2.0;
	threepoints.add_point(p, 0);
	int cnt = 0;
	for (int k = 0; k < K; k++)
	{
		threepoints.init_plusplus();
		if (threepoints.get_center(0).coords[0] == 1.0)
			cnt++;
	}
	assert(("[NOK]", std::fabs(cnt / (double)K - prob_threepoints) < delta));

	// test case 2
	const double prob_twoclusters = 0.125;
	cloud twoclusters(1, 4, 2);
	p.coords[0] = 0.0;
	twoclusters.add_point(p, 0);
	p.coords[0] = 0.0;
	twoclusters.add_point(p, 0);
	p.coords[0] = 1.0;
	twoclusters.add_point(p, 0);
	p.coords[0] = 2.0;
	twoclusters.add_point(p, 0);
	cnt = 0;
	for (int k = 0; k < K; k++)
	{
		twoclusters.init_plusplus();
		if (twoclusters.get_center(1).coords[0] == 1.0)
			cnt++;
	}
	assert(("[NOK]", std::fabs(cnt / (double)K - prob_twoclusters) < delta));

	std::cout << "\t\t[OK]" << std::endl;
}

void test_init_random_partition()
{
	// number of random experiments
	const int K = 10000;
	// tolerance in probability
	const double delta = 0.0625;

	// dimenstion used for tests
	point::set_dim(1);

	// temporary container
	point p;

	std::cout << "Testing function init_random_parition()...";

	const double prob_threepoints = 0.3333;
	cloud threepoints(1, 3, 3);
	p.coords[0] = 0.0;
	threepoints.add_point(p, 0);
	p.coords[0] = 1.0;
	threepoints.add_point(p, 0);
	p.coords[0] = 2.0;
	threepoints.add_point(p, 0);
	int cnt = 0;
	for (int k = 0; k < K; k++)
	{
		threepoints.init_random_partition();
		if (threepoints.get_point(2).label == 1)
			cnt++;
	}
	assert(("[NOK]", std::fabs(cnt / (double)K - prob_threepoints) < delta));

	std::cout << "\t[OK]" << std::endl;
}

void test_kmeans()
{
	std::cout << "Testing kmeans()..." << std::endl;
	// dimension used for tests
	point::set_dim(1);

	// temporary container
	point p;

	// test case 1
	const double dist_onepoint_zerodist = 0.0;
	cloud onepoint_zerodist(1, 1, 1);
	p.coords[0] = 0.0;
	onepoint_zerodist.add_point(p, 0);
	onepoint_zerodist.kmeans();
	double after = onepoint_zerodist.intracluster_variance();
	std::cout << "Variance attendus: 0 et variance obtenus: " << after << std::endl;

	// test case 2
	const double dist_onepoint_posdist = 0.25;
	cloud onepoint_posdist(1, 1, 1);
	p.coords[0] = 0.5;
	onepoint_posdist.add_point(p, 0);
	onepoint_posdist.kmeans();
	after = onepoint_posdist.intracluster_variance();
	std::cout << "Variance attendus: 0 et variance obtenus: " << after << std::endl;

	// test case 3
	const double dist_twopoints = 0.625;
	cloud twopoints(1, 2, 1);
	p.coords[0] = -1.0;
	twopoints.add_point(p, 0);
	p.coords[0] = 0.5;
	twopoints.add_point(p, 0);
	p.coords[0] = -0.5;
	twopoints.set_center(p, 0);
	twopoints.kmeans();
	after = twopoints.intracluster_variance();
	std::cout << "Variance attendus: 0.5625 et variance obtenus: " << after << std::endl;

	// test case 4
	cloud twoclusters(1, 4, 2);
	p.coords[0] = -1.0;
	twoclusters.add_point(p, 0);
	p.coords[0] = 0.5;
	twoclusters.add_point(p, 0);
	p.coords[0] = -0.5;
	twoclusters.set_center(p, 0);
	p.coords[0] = -2.0;
	twoclusters.add_point(p, 1);
	p.coords[0] = 2.0;
	twoclusters.add_point(p, 1);
	twoclusters.kmeans();
	after = twoclusters.intracluster_variance();
	std::cout << "Variance attendus: 0.40625 et variance obtenus: " << after << std::endl;
}