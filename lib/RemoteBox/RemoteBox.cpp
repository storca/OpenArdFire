#include "RemoteBox.h"

/**
 * Declare a RemoteBox
 * @param address  I2C address of the remote box
 * @param firstCue First cue of the remote box eg : 58
 * @param lastCue  Last cue of the remote box eg : 73 //Fix to come here
 */
RemoteBox::RemoteBox(unsigned int address, unsigned int firstCue, unsigned int lastCue)
{
  this->_cuesRange[0] = firstCue;
  this->_cuesRange[1] = lastCue;

  this->_i2cAddress = address;

}

void RemoteBox::begin()
{
  //Initialise MCP with it's address
  this->mcp.begin(this->_i2cAddress);

  //Setting MCP pins as outputs and are set to low
  for(int cue=0; cue<=15; cue++)
  {
    this->mcp.pinMode(cue, OUTPUT);
    this->mcp.digitalWrite(cue, LOW);
  }
}
RemoteBox::~RemoteBox()
{

}
