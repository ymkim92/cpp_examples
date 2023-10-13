#include "fsm.h"
#include <stdio.h>
#include <iostream>
#include <memory>
#include <string>
using namespace Patterns;
bool KeyPresses[255];
enum class TurnstileStateType {
  LOCKED,
  UNLOCKED,
};
class TurnstileLockedState : public State<TurnstileStateType> {
public:
  TurnstileLockedState(FiniteStateMachine<TurnstileStateType> &fsm)
      : State<TurnstileStateType>(fsm, TurnstileStateType::LOCKED, "Locked") {}
  void enter() {
    State<TurnstileStateType>::enter();
    printf("Turnstile state: LOCKED\n");
  }
  void update() {
    State<TurnstileStateType>::update();
    if (KeyPresses['c']) {
      printf(" - coin inserted, unlocking turnstile\n");
      mFsm.setCurrentState(TurnstileStateType::UNLOCKED);
    } else {
      printf("Please insert a coin by pressing c to unlock the turnstile.\n");
    }
  }
};
class TurnstileUnLockedState : public State<TurnstileStateType> {
public:
  TurnstileUnLockedState(FiniteStateMachine<TurnstileStateType> &fsm)
      : State<TurnstileStateType>(fsm, TurnstileStateType::UNLOCKED,
                                  "Unlocked") {}
  void enter() {
    State<TurnstileStateType>::enter();
    printf("Turnstile state: UNLOCKED\n");
  }
  void update() {
    State<TurnstileStateType>::update();
    if (KeyPresses['p']) {
      printf(" - pushed, locking turnstile\n");
      mFsm.setCurrentState(TurnstileStateType::LOCKED);
    }
  }
};
int main(int argc, char *argv) {
  printf("--------------------------------------------\n");
  printf("An example demo for the Finite State Machine\n");
  printf("--------------------------------------------\n");
  printf("  Press the c key to insert coin.\n");
  printf("  Press the p key to open the turnstile\n");
  printf("  Press the q key to quit\n");
  printf("--------------------------------------------\n");
  // create the state machine for the turnstile.
  std::unique_ptr<FiniteStateMachine<TurnstileStateType>> fsm =
      std::make_unique<FiniteStateMachine<TurnstileStateType>>();
  State<TurnstileStateType> &state_locked =
      fsm->add<TurnstileLockedState>(TurnstileStateType::LOCKED);
  State<TurnstileStateType> &state_unlocked =
      fsm->add<TurnstileLockedState>(TurnstileStateType::UNLOCKED);
  fsm->setCurrentState(TurnstileStateType::LOCKED);
  bool done = false;
  while (!done) {
    char input = getchar();

    KeyPresses[input] = true;
    if (KeyPresses['q']) {
      done = true;
      break;
    }
    fsm->update();
    for (int i = 0; i < 255; ++i)
      KeyPresses[i] = false;
  }
  printf("You have exited the program. Good bye!\n");
}