#include <all_shop_states.hpp>
#include "ortools/linear_solver/linear_solver.h"
#include <iostream>

ShopState& RestockingSuggestion::getInstance() {
  static RestockingSuggestion singleton;
  return singleton;
}

namespace optimizer{
std::vector<double> MipVarArray(long long n, std::vector<long double> buying_price,std::vector<long double> selling_price,std::vector<long double> holding_cost,std::vector<long long> forecast,long double capital) {

  std::vector<double> solution;
  // Create the mip solver with the SCIP backend.
  std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));
  if (!solver) {
    LOG(WARNING) << "SCIP solver unavailable.";
    return solution;
  }

  const double infinity = solver->infinity();
  // x[j] is an array of non-negative, integer variables.
  std::vector<const MPVariable*> x(n);
  for (int j = 0; j < n; ++j) {
    x[j] = solver->MakeIntVar(forecast[j], infinity, "");
  }
  //LOG(INFO) << "Number of variables = " << solver->NumVariables();

  /*
  // Create the constraints.
  for (int i = 0; i < data.num_constraints; ++i) {
    MPConstraint* constraint = solver->MakeRowConstraint(0, data.bounds[i], "");
    for (int j = 0; j < data.num_vars; ++j) {
      constraint->SetCoefficient(x[j], data.constraint_coeffs[i][j]);
    }
  }
  LOG(INFO) << "Number of constraints = " << solver->NumConstraints();
  */
  MPConstraint* const c0 = solver->MakeRowConstraint(-infinity, capital, "c0");
  for (int j =0 ; j<n ;j++){
    c0 -> SetCoefficient(x[j], buying_price[j]) ;
  }


  // Create the objective function.
  MPObjective* const objective = solver->MutableObjective();
  for (int j = 0; j < n; ++j) {
    objective->SetCoefficient(x[j], selling_price[j]-buying_price[j]-holding_cost[j]);
  }
  objective->SetMaximization();

  const MPSolver::ResultStatus result_status = solver->Solve();

  // Check that the problem has an optimal solution.
  if (result_status != MPSolver::OPTIMAL) {
    LOG(FATAL) << "The problem does not have an optimal solution.";
  }
  //LOG(INFO) << "Solution:";
  //LOG(INFO) << "Optimal objective value = " << objective->Value();

  for (int j = 0; j < n; ++j) {
    //LOG(INFO) << "x[" << j << "] = " << x[j]->solution_value();
    solution.push_back(x[j]->solution_value());
  }
  return solution;
}

}  // namespace optimizer

void RestockingSuggestion::enter(Shop* shop) {}
void RestockingSuggestion::exit(Shop* shop) {}

 
// For Debugging
/*
int main(int argc, char** argv) {
  std::vector<double>solution;
  long long n = 5;
  long double capital = 100000;
  std::vector<long double> selling_price = {100,200,50,20,10};
  std::vector<long double> buying_price = {80,160,40,12,7};
  std::vector<long double> holding_cost = {20,30,40,50,60};
  std::vector<long long> forecast = {10,15,20,25,40};
  solution = optimizer::MipVarArray(n,buying_price,selling_price,holding_cost,forecast,capital);
  for (int i =0 ;i<n;i++){
    std::cout<<solution[i]<<std::endl;
  }
  return EXIT_SUCCESS;
}
*/
