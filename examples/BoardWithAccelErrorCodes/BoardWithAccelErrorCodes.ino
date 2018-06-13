#include <DSPI.h>
#include <OpenBCI_32bit_Library.h>
#include <OpenBCI_32Bit_Library_Definitions.h>

void setup() {
  // Notify the board we want to use accel data
  board.useAccel(true);

  // Notify the board we want to use error codes
  board.useErrorCodes(true);

  // Clear the error codes
  board.errorCodesClear();

  // Bring up the OpenBCI Board
  board.begin();

  // Give extra time for start up message to finish
  delay(100);
  
  // Print out error codes after start up
  board.errorCodesReport(); board.sendEOT();
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
  if (board.hasDataSerial0()) board.processChar(board.getCharSerial0());
  if (board.hasDataSerial1()) board.processChar(board.getCharSerial1());
  board.loop();
}

