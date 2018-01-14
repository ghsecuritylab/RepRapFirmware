/*
 * Duet3DFilamentSensor.h
 *
 *  Created on: 20 Jul 2017
 *      Author: David
 *
 *  This is the base class for filament monitors that use the Duet3D protocol for sending 16-bit words to the Duet.
 */

#ifndef SRC_FILAMENTSENSORS_DUET3DFILAMENTMONITOR_H_
#define SRC_FILAMENTSENSORS_DUET3DFILAMENTMONITOR_H_

#include "FilamentMonitor.h"

class Duet3DFilamentMonitor : public FilamentMonitor
{
public:
	Duet3DFilamentMonitor(int type);

	void Interrupt() override;

protected:
	virtual void OnStartBitReceived() = 0;
	virtual void ProcessReceivedWord(uint16_t val) = 0;
	void InitReceiveBuffer();
	void PollReceiveBuffer();
	bool IsReceiving() const;

private:
	static constexpr size_t EdgeCaptureBufferSize = 64;				// must be a power of 2

	// Buffer used to capture received data, and associated info
	uint32_t edgeCaptures[EdgeCaptureBufferSize];
	size_t edgeCaptureReadPointer;
	volatile size_t edgeCaptureWritePointer;

	enum class RxdState : uint8_t
	{
		waitingForStartBit,
		waitingForEndOfStartBit,
		waitingForNibble,
		errorRecovery1,
		errorRecovery2,
		errorRecovery3,
		errorRecovery4
	};

	RxdState state;
	uint32_t startBitLength;
	uint32_t errorRecoveryStartTime;
	size_t lastBitChangeIndex;
	uint16_t valueBeingAssembled;
	uint8_t nibblesAssembled;
};

#endif /* SRC_FILAMENTSENSORS_DUET3DFILAMENTMONITOR_H_ */