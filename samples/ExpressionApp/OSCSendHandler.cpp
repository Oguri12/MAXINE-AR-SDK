#include "OSCSendHandler.h"

OSCSendHandler::OSCSendHandler() {
	transmitSocket = NULL;
}

OSCSendHandler::~OSCSendHandler() {
	delete transmitSocket;
}

void OSCSendHandler::connectSocket(std::string ip_address, int port) {
	
	transmitSocket = new UdpTransmitSocket(IpEndpointName(ip_address.c_str(),port));
	printf("Socket Initialized : %s Port : %i\n\n", ip_address.c_str(), port);
}

void OSCSendHandler::send(std::vector<float> expressions) {
	if (!transmitSocket)
		return;

	if (expressions.size() <= 6) 
		return;

	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

	p.Clear();
	p << osc::BeginBundle();

	for (size_t i = 0; i < 52; i++) {

		if (i == 0) {
			// BrowInnerUp
			p << osc::BeginMessage("/VMC/Ext/Blend/Val")
				<< _psNames[i] << (0.5f * (expressions[2] + expressions[3]))
				<< osc::EndMessage;

		} else if (i == 19) {
			// CheekPuff
			p << osc::BeginMessage("/VMC/Ext/Blend/Val")
				<< _psNames[i] << (0.5f * (expressions[6] + expressions[7]))
				<< osc::EndMessage;

		} else if (i == 51) {
			// TongueOut
			p << osc::BeginMessage("/VMC/Ext/Blend/Val")
				<< _psNames[i] << 0.0f
				<< osc::EndMessage;

		} else {
			p << osc::BeginMessage("/VMC/Ext/Blend/Val")
				<< _psNames[i] << expressions[_psExprIndex[i]]
				<< osc::EndMessage;
		}
	}

	p << osc::BeginMessage("/VMC/Ext/Blend/Apply") << osc::EndMessage;

	p << osc::EndBundle;

	if (p.IsReady())
		transmitSocket->Send(p.Data(), p.Size());

}


const char *OSCSendHandler::_psNames[] = {
  "BrowInnerUp" ,				//  0.5 * (browInnerUp_L + browInnerUp_R) 0.5 * (2, + , 3)
  "BrowDownLeft" ,				// browDown_L	0
  "BrowDownRight",				// browDown_R	1
  "BrowOuterUpLeft" ,			// browOuterUp_L	4
  "BrowOuterUpRight"  ,			// browOuterUp_R	5
  "EyeLookUpLeft" ,				// eyeLookUp_L		18
  "EyeLookUpRight" ,			// eyeLookUp_R		19
  "EyeLookDownLeft" ,			// eyeLookDown_L	12
  "EyeLookDownRight" ,			// eyeLookDown_R	13
  "EyeLookInLeft",				// eyeLookIn_L		14
  "EyeLookInRight"  ,			// eyeLookIn_R		15
  "EyeLookOutLeft" ,			// eyeLookOut_L		16
  "EyeLookOutRight"  ,			// eyeLookOut_R		17
  "EyeBlinkLeft" ,				// eyeBlink_L		10
  "EyeBlinkRight"  ,			// eyeBlink_R		11
  "EyeSquintLeft" ,				// eyeSquint_L		20
  "EyeSquintRight"  ,			// eyeSquint_R		21
  "EyeWideLeft" ,				// eyeWide_L		22
  "EyeWideRight" ,				// eyeWide_R		23
  "CheekPuff" ,					// 0.5 * (cheekPuff_L + cheekPuff_R)	0.5 * (6, + , 7)
  "CheekSquintLeft"   ,			// cheekSquint_L	8
  "CheekSquintRight"  ,			// cheekSquint_R	9
  "NoseSneerLeft"  ,			// noseSneer_L		51
  "NoseSneerRight"   ,			// noseSneer_R		52
  "JawOpen"  ,					// jawOpen			26
  "JawForward"  ,				// jawForward		24
  "JawLeft"   ,					// jawLeft			25
  "JawRight"   ,				// jawRight			27
  "MouthFunnel"   ,				// mouthFunnel		33
  "MouthPucker"  ,				// mouthPucker		39
  "MouthLeft" ,					// mouthLeft		34
  "MouthRight"  ,				// mouthRight		40
  "MouthRollUpper"  ,			// mouthRollUpper	42
  "MouthRollLower"  ,			// mouthRollLower	41
  "MouthShrugUpper" ,			// mouthShrugUpper	44
  "MouthShrugLower"  ,			// mouthShrugLower	43
  "MouthClose" ,				// mouthClose		28
  "MouthSmileLeft" ,			// mouthSmile_L		45
  "MouthSmileRight"   ,			// mouthSmile_R		46
  "MouthFrownLeft"  ,			// mouthFrown_L		31
  "MouthFrownRight"   ,			// mouthFrown_R		32
  "MouthDimpleLeft"   ,			// mouthDimple_L	29
  "MouthDimpleRight" ,			// mouthDimple_R	30
  "MouthUpperUpLeft"  ,			// mouthUpperUp_L	49
  "MouthUpperUpRight"   ,		// mouthUpperUp_R	50
  "MouthLowerDownLeft"   ,		// mouthLowerDown_L	35
  "MouthLowerDownRight"  ,		// mouthLowerDown_R	36
  "MouthPressLeft"   ,			// mouthPress_L		37
  "MouthPressRight"  ,			// mouthPress_R		38
  "MouthStretchLeft" ,			// mouthStretch_L	47
  "MouthStretchRight" ,			// mouthStretch_R	48
  "TongueOut"  					// 0
};

const int OSCSendHandler::_psExprIndex[] = {
	-1,0,1,4,5,18,19,12,13,14,15,16,17,10,11,20,21,22,23,-1,8,9,51,52,26,24,25,27,33,39,34,40,42,41,44,43,28,45,46,31,32,29,30,49,50,35,36,37,38,47,48,-1
};