#include "ee24c64.hpp"

ee24c64::ee24c64() {
    this->si = new SlowSoftI2CMaster(PA13, PA15, true);
}

bool ee24c64::init() {
    return this->si->i2c_init();
}

boolean ee24c64::readEEPROM(unsigned long address, uint8_t *byte) {
  if (!si->i2c_start((EEI2Caddr<<1)|I2C_WRITE)) return false;
  if (!si->i2c_write((address>>8)&0xFF)) return false;
  if (!si->i2c_write(address&0xFF)) return false;
  if (!si->i2c_rep_start((EEI2Caddr<<1)|I2C_READ))return false;
  *byte = si->i2c_read(true);
  si->i2c_stop();
  delay(10);
  return true;
}

boolean ee24c64::writeEEPROM(long unsigned address, uint8_t byte) {
  if (!si->i2c_start((EEI2Caddr<<1)|I2C_WRITE)) return false;
  if (!si->i2c_write((address>>8)&0xFF)) return false;
  if (!si->i2c_write(address&0xFF)) return false;
  if (!si->i2c_write(byte)) return false;
  si->i2c_stop();
  delay(10);
  return true;
}

boolean ee24c64::deleteEEPROM(long unsigned from, unsigned long to, uint8_t byte, boolean poll) {
  unsigned long tempto, i;
  boolean firstpage = true;

  while (from <= to)
  {
    tempto = ((from/128)+1)*128-1;
    if (tempto > to) tempto = to;
    if (firstpage || !poll)
      if (!si->i2c_start((EEI2Caddr<<1)|I2C_WRITE)) 
	      return false;
      else
        si->i2c_start_wait((EEI2Caddr<<1)|I2C_WRITE);
    
    if (!si->i2c_write((from>>8)&0xFF)) return false;
    if (!si->i2c_write(from&0xFF)) return false;
    
    for (i=from; i<=tempto; i++)
      if (!si->i2c_write(byte)) return false;
    si->i2c_stop();

    if (!poll) delay(6);
    from = tempto+1;
    firstpage = false;
  }
  return true;
}