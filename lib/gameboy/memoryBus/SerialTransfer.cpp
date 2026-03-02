// ****************************************************************************** //
//   _______   __                              __       __                        //
//  /       \ /  |                            /  \     /  |                       //
//  $$$$$$$  |$$ |  ______   __    __         $$  \   /$$ |  ______   _______     //
//  $$ |__$$ |$$ | /      \ /  |  /  | ______ $$$  \ /$$$ | /      \ /       \    //
//  $$    $$/ $$ | $$$$$$  |$$ |  $$ |/      |$$$$  /$$$$ | $$$$$$  |$$$$$$$  |   //
//  $$$$$$$/  $$ | /    $$ |$$ |  $$ |$$$$$$/ $$ $$ $$/$$ | /    $$ |$$ |  $$ |   //
//  $$ |      $$ |/$$$$$$$ |$$ \__$$ |        $$ |$$$/ $$ |/$$$$$$$ |$$ |  $$ |   //
//  $$ |      $$ |$$    $$ |$$    $$ |        $$ | $/  $$ |$$    $$ |$$ |  $$ |   //
//  $$/       $$/  $$$$$$$/  $$$$$$$ |        $$/      $$/  $$$$$$$/ $$/   $$/    //
//                          /  \__$$ |                                            //
//                          $$    $$/                                             //
//                           $$$$$$/                                              //
//                                                                                //
//                            By: K1ngmar and rvan-mee                            //
// ****************************************************************************** //

#include <play-man/gameboy/memoryBus/SerialTransfer.hpp>
#include <play-man/utility/UtilFunc.hpp>
#include <play-man/logger/Logger.hpp>
#include <assert.h>
#include <sstream>

constexpr uint16_t SerialTransferDataAddress = 0xFF01;
constexpr uint16_t SerialTransferControlAddress = 0xFF02;

namespace GameBoy {

// SerialTransfer::SerialTransfer(const bool& _cgbMode) : SC(0x00), cgbMode(_cgbMode)
// {
// }

SerialTransfer::SerialTransfer() : SC(0x00)
{
}

SerialTransfer::~SerialTransfer()
{
}

void SerialTransfer::WriteByte(const uint16_t address, uint8_t value)
{
    if (address == SerialTransferDataAddress)
    {
        LOG_DEBUG("Setting serial data: " + Utility::IntAsHexString(value));
        serialData = value;
    }
    else if (address == SerialTransferControlAddress)
    {
        std::stringstream ss;

        ss << "Setting Serial Transfer Control: ";
        ss << "Transfer: " << ((value & GetEnumAsValue(SerialTransferControlFlag::TRANSFER_ENABLE)) != 0 ? "enabled" : "disabled");
        ss << " - Clock speed (cgb only): " << ((value & GetEnumAsValue(SerialTransferControlFlag::CLOCK_SPEED)) != 0 ? "high speed" : "normal speed");
        ss << " - Clock select: " << ((value & GetEnumAsValue(SerialTransferControlFlag::CLOCK_SELECT)) != 0 ? "internal clock" : "external clock");
        
        LOG_DEBUG(ss.str());

        if ((value & GetEnumAsValue(SerialTransferControlFlag::TRANSFER_ENABLE)) != 0 )
            assert(false && "Serial transfer is not yet implemented");

        SC = value;
    }
    else
    {
        assert(false && "Writing to an unsupported memory address within SerialTransfer");
    }
}

uint8_t SerialTransfer::ReadByte(const uint16_t address)
{
    if (address == SerialTransferDataAddress)
    {
        assert(false && "Cannot read from the serial data stream yet as this is unimplemented");
    }
    else if (address == SerialTransferControlAddress)
    {
        return SC;
    }
    else
    {
        assert(false && "Reading from an unsupported memory address within SerialTransfer");
    }
}
}