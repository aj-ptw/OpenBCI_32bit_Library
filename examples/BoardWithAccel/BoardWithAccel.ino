#include <DSPI.h>
#include <OpenBCI_32bit_Library.h>
#include <OpenBCI_32Bit_Library_Definitions.h>

void setup() {
  // Bring up the OpenBCI Board
  board.begin();

  // Notify the board we want to use accel data
  board.useAccel(true);

  board.iSerial0.tx = false;
  board.iSerial1.tx = false;

  board.setSampleRate(board.SAMPLE_RATE_250);

  board.setChannelsToDefault();

  board.streamStart();
}

void loop() {
  // The main dependency of this single threaded microcontroller is to
  //  stream data from the ADS.
  if (board.streaming) {
    if (board.channelDataAvailable) {
      // Read from the ADS(s), store data, set channelDataAvailable flag to false
      board.updateChannelData();

      // Check to see if accel has new data
      if(board.accelHasNewData()){
        // Get new accel data
        board.accelUpdateAxisData();
      }

      // Send standard packet with channel data
      board.sendChannelData();
    }
  }

  // Check the serial ports for new data
  if (board.hasDataSerial()) board.processChar(board.getCharSerial());  
  if (board.hasDataSerial0()) board.processChar(board.getCharSerial0());
  if (board.hasDataSerial1()) board.processChar(board.getCharSerial1());
  board.loop();
}
