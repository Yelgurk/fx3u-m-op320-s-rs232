#include "baseNode.hpp"

BaseNode::BaseNode() {}

void BaseNode::init(long OPaddr)
{
    this->OPaddr = OPaddr;
    this->isOPCommand = true;
}

void BaseNode::init(long OPaddr, bool isSavedInEE, long EEaddr)
{
    this->OPaddr = OPaddr;
    if (isSavedInEE)
    {
        this->isSavedInEE = true;
        this->EEaddr = EEaddr;
    }
}

long BaseNode::getOPAddr() {
    return this->OPaddr;
}

void BaseNode::setScale(byte scale){
    this->OPvalueScale = scale;
}

void BaseNode::setValue(byte value = 0)
{
    if (this->isOPCommand)
        this->value = 0;
    else
        this->value = value;
}

void BaseNode::setValue(bool value) {
    this->setValue(value ? (byte)1 : (byte)0);
}

void BaseNode::setAndWriteToEE(byte value)
{
    this->setValue(value);
    this->writeToEE();
}

void BaseNode::setAndWriteToOP(byte value)
{
    this->setValue(value);
    this->writeToOP();
}

byte BaseNode::getValue() {
    return this->value;
}

bool BaseNode::writeToEE()
{
    if (this->isSavedInEE)
    {
        ++Global::EE_currSectCyclCnt;
        Global::EE.writeEEPROM(Global::EE_startAddr + Global::EE_cyclCnt_HB_Addr, highByte(Global::EE_currSectCyclCnt));
        Global::EE.writeEEPROM(Global::EE_startAddr + Global::EE_cyclCnt_LB_Addr, lowByte(Global::EE_currSectCyclCnt));
        return Global::EE.writeEEPROM(Global::EE_startAddr + this->EEaddr, this->value);
    }
    else
        return false;
}

bool BaseNode::readFromEE()
{
    if (this->isSavedInEE)
        return Global::EE.readEEPROM(Global::EE_startAddr + this->EEaddr, &value);
    else
        return false;
}

void BaseNode::writeToOP()
{
    if (this->isOPCommand)
        Global::OP.writeSingleCoil(this->OPaddr, this->value);
    else
    {
        uint16_t preScaledValue = this->value * this->OPvalueScale;
        Global::OP.writeSingleRegister(this->OPaddr, preScaledValue);
    }
}

uint8_t BaseNode::readFromOP()
{
    if (this->isOPCommand)
        Global::OP.readCoils(this->OPaddr, 1);
    else
        Global::OP.readHoldingRegisters(this->OPaddr, 1);
    this->value = Global::OP.getResponseBuffer(0);

    return this->value;
}

void BaseNode::fromEEtoOP()
{
    readFromEE();
    writeToOP();
}

void BaseNode::fromOPtoEE()
{
    readFromOP();
    writeToEE();
}