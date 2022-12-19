#ifndef ORG_H
#define ORG_H

#include "CPU.h"
#include "OrgState.h"
#include "emp/Evolve/World_structure.hpp"

class Organism {
  CPU cpu;
  std::string task_solved;
public:
  Organism(OrgWorld *world, double points = 0.0, std::string task = "none") : cpu(world) {
    SetPoints(points);
    SetTaskSolved(task);

  }
  void SetPoints(double _in) { cpu.state.points = _in; }
  void SetTaskSolved(std::string _task) {cpu.state.solved_task = _task;}
  void AddPoints(double _in) { cpu.state.points += _in; }
  double GetPoints() {return cpu.state.points;}
  bool GetSolved() {return cpu.state.solved;}

  void Reset() {
    cpu.Reset();
  }

  void Mutate() {
    cpu.Mutate();
  }

  std::optional<Organism> CheckReproduction() {
    if(Get_Solved()=="ceil"){
     std::cout << "Org_Get_Task:   " << Get_Solved() << std::endl;
    }
    if (GetPoints() > 20) {
      Organism offspring = *this;
      offspring.Reset();
      offspring.Mutate();
      AddPoints(-20);
      return offspring;
    }
    
    return {};
  }

  void Process(emp::WorldPosition current_location) {
    cpu.state.current_location = current_location;
    cpu.RunCPUStep(10);
    //TODO: Run the CPU!

  }

  std::string Get_Solved(){
    
    return cpu.state.solved_task;
  }
  void Set_Solved(std::string task){
    task_solved = task;
  }


  void PrintGenome() {
    std::cout << "program ------------" << std::endl;
    cpu.PrintGenome();
    std::cout << "end ---------------" << std::endl;
  }
};

#endif