#ifndef TYPES_H
#define TYPES_H

enum Phases { NoPhase=0, L1=1, L2=2, L3=3 };
enum Connections { ACIn1=0, ACOut=1, ACIn2=2, NoConn=255 };
enum Positions { None, ACIn1_L1, ACIn1_L2, ACIn1_L3, ACIn2_L1, ACIn2_L2,
				 ACIn2_L3, ACOut_L1, ACOut_L2, ACOut_L3 };
#endif // TYPES_H
