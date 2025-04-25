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

#include <play-man/gameboy/cartridge/MBC3.hpp>
#include <play-man/logger/Logger.hpp>

/*     Read Address Ranges     */
static constexpr uint16_t RomBank00Start = 0x0000;
static constexpr uint16_t RomBank00End = 0x3FFF;
static constexpr uint16_t RomBankedStart = 0x4000;
static constexpr uint16_t RomBankedEnd = 0x7FFF;

/*     Read/Write Address Ranges     */
static constexpr uint16_t RamBankOrTimerStart = 0xA000;
static constexpr uint16_t RamBankOrTimerEnd = 0xBFFF;

/*     Write Address Ranges     */
static constexpr uint16_t TimerRegistersStart = 0x0000;
static constexpr uint16_t TimerRegistersEnd = 0x1FFF;
static constexpr uint16_t RomBankNumberStart = 0x2000;
static constexpr uint16_t RomBankNumberEnd = 0x3FFF;
static constexpr uint16_t RamBankNumberOrTimerSelectStart = 0x4000;
static constexpr uint16_t RamBankNumberOrTimerSelectEnd = 0x5FFF;
static constexpr uint16_t LatchClockDataStart = 0x6000;
static constexpr uint16_t LatchClockDataEnd = 0x7FFF;

/*     Ranges used to select what is mapped at the ramBankOrTimer range     */
static constexpr uint8_t RamBankSelectStart = 0x00;
static constexpr uint8_t RamBankSelectEnd = 0x07;
static constexpr uint8_t TimerRegisterSelectStart = 0x08;
static constexpr uint8_t TimerRegisterSelectEnd = 0x0C;

/*     Default Register Values     */
static constexpr uint8_t DefaultRomBankNumber = 0x01;
static constexpr uint8_t DefaultRamOrTimerSelect = 0x00;
static constexpr uint8_t DefaultLatchClockData = 0x00;
static constexpr bool    DefaultRamAndTimerEnabled = false;
static constexpr bool    DefaultHasRTC = false;

/*     Masks     */
static constexpr uint8_t EnableRamAndTimerMask = 0b0000'1111;
static constexpr uint8_t RomBankNumberMask = 0b0111'1111;

/*     Misc     */
static constexpr uint8_t EnableRamAndTimerValue = 0x0A;


namespace GameBoy {

    MBC3Cartridge::MBC3Cartridge(std::unique_ptr<Rom> rom) : ACartridge(std::move(rom))
    {
        /*     Control Registers     */
        ramAndTimerEnabled = DefaultRamAndTimerEnabled;
        ramOrTimerSelect = DefaultRamOrTimerSelect;
        latchClockData = DefaultLatchClockData;
        romBankNumber = DefaultRomBankNumber;
        hasRTC = DefaultHasRTC;

        const CartridgeType cType = GetType();
        if (cType == CartridgeType::MBC3_TIMER_BATTERY || cType == CartridgeType::MBC3_TIMER_RAM_BATTERY)
            hasRTC = true;
    }

    uint8_t MBC3Cartridge::ReadRAM(const uint16_t address)
    {
        if (!ramAndTimerEnabled)
        {
            LOG_DEBUG(READ_RAM_DISABLED);
            return OpenBusValue;
        }

        if (ramOrTimerSelect >= GetRamBankCount())
        {
            LOG_DEBUG(RAM_BANK_INVALID);
            return OpenBusValue;
        }

        // The ramOrTimerSelect register specifies which bank to read from.
        return ramBanks[ramOrTimerSelect][address - RamBankOrTimerStart];
    }

    uint8_t MBC3Cartridge::ReadRTC()
    {
        if (!ramAndTimerEnabled)
        {
            LOG_DEBUG(READ_TIMER_DISABLED);
            return OpenBusValue;
        }

        // The value inside the ramOrTimerSelect register specifies which RTC register has to be read from.
        switch (ramOrTimerSelect)
        {
            case GetEnumAsValue(RTCRegisters::Seconds): return RTC.GetSecondsLatched();
            case GetEnumAsValue(RTCRegisters::Minutes): return RTC.GetMinutesLatched();
            case GetEnumAsValue(RTCRegisters::Hours): return RTC.GetHoursLatched();
            case GetEnumAsValue(RTCRegisters::DaysLowerBits): return RTC.GetDaysLowerLatched();
            case GetEnumAsValue(RTCRegisters::DaysUpperAndFlags): return RTC.GetDaysUpperAndFlagsLatched();
        }

        assert(false); // Should be unreachable
    }

    uint8_t MBC3Cartridge::ReadByte(const uint16_t address)
    {
        if (address >= RomBank00Start && address <= RomBank00End)
        {
            return rom->ReadFromBank(0, address);
        }
        else if (address >= RomBankedStart && address <= RomBankedEnd)
        {
            return rom->ReadFromBank(romBankNumber, address);
        }
        else if (address >= RamBankOrTimerStart && address <= RamBankOrTimerEnd)
        {
            // This address range is used for both the ram and the RTC registers.

            if (ramOrTimerSelect >= RamBankSelectStart && ramOrTimerSelect <= RamBankSelectEnd)
            {
                // The RAM bank is selected based on the value inside the ramOrTimerSelect register.
                return ReadRAM(address);
            }
            else if (ramOrTimerSelect >= TimerRegisterSelectStart && ramOrTimerSelect <= TimerRegisterSelectEnd)
            {
                // RTC reads are done based on the value inside the ramOrTimerSelect register.
                return ReadRTC();
            }

            LOG_ERROR("Cartridge: ramOrTimerSelect register contains an invalid value");
            return OpenBusValue;
        }

        LOG_ERROR(READ_OUT_OF_RANGE);
        return OpenBusValue;
    }

    void    MBC3Cartridge::WriteRAM(const uint16_t address, const uint8_t value)
    {
        if (!ramAndTimerEnabled)
        {
            LOG_DEBUG(WRITE_RAM_DISABLED);
            return ;
        }

        if (ramOrTimerSelect >= GetRamBankCount())
        {
            LOG_DEBUG(RAM_BANK_INVALID);
            return ;
        }

        // The ramOrTimerSelect register specifies which bank to write to.
        ramBanks[ramOrTimerSelect][address - RamBankOrTimerStart] = value;
    }

    void    MBC3Cartridge::WriteRTC(const uint8_t value)
    {
        if (!ramAndTimerEnabled)
        {
            LOG_DEBUG(WRITE_TIMER_DISABLED);
            return ;
        }

        // The value inside the ramOrTimerSelect register specifies which RTC register has to be written to.
        switch (ramOrTimerSelect)
        {
            case GetEnumAsValue(RTCRegisters::Seconds): RTC.SetSecondsInternal(value); return ;
            case GetEnumAsValue(RTCRegisters::Minutes): RTC.SetMinutesInternal(value); return ;
            case GetEnumAsValue(RTCRegisters::Hours): RTC.SetHoursInternal(value); return ;
            case GetEnumAsValue(RTCRegisters::DaysLowerBits): RTC.SetDaysLowerInternal(value); return ;
            case GetEnumAsValue(RTCRegisters::DaysUpperAndFlags): RTC.SetDaysUpperAndFlagsInternal(value); return ;
        }
    }

    void    MBC3Cartridge::WriteByte(const uint16_t address, const uint8_t value)
    {
        if (address >= TimerRegistersStart && address <= TimerRegistersEnd)
        {
            ramAndTimerEnabled = ((value & EnableRamAndTimerMask) == EnableRamAndTimerValue);
        }
        else if (address >= RomBankNumberStart && address <= RomBankNumberEnd)
        {
            // Nearly the same as the MBC1 cartridge, except that whole 7 bits
            // of the bank number are directly written to the register.
            romBankNumber = value & RomBankNumberMask;
            if (romBankNumber == 00)
                romBankNumber = 01;
        }
        else if (address >= RamBankNumberOrTimerSelectStart && address <= RamBankNumberOrTimerSelectEnd)
        {
            ramOrTimerSelect = value;
        }
        else if (address >= LatchClockDataStart && address <= LatchClockDataEnd)
        {
            // To latch the clock's counter registers to the latched ones a write of 0x00 followed by a write
            // of 0x01 must be done on this address range.
            if (latchClockData == 0x00 && value == 0x01)
                RTC.LatchInternalRegisters();
            latchClockData = value;
        }
        else if (address >= RamBankOrTimerStart && address <= RamBankOrTimerEnd)
        {
            // This address range is used for both the ram and the RTC registers,
            // the ramOrTimerSelect register controls which of the 2 will be accessed.
            if (ramOrTimerSelect >= RamBankSelectStart && ramOrTimerSelect <= RamBankSelectEnd)
            {
                WriteRAM(address, value);
            }
            else if (ramOrTimerSelect >= TimerRegisterSelectStart && ramOrTimerSelect <= TimerRegisterSelectEnd)
            {
                WriteRTC(value);
            }

            LOG_ERROR("Cartridge: ramOrTimerSelect register contains an invalid value");
        }
        else
            LOG_DEBUG(WRITE_OUT_OF_RANGE);
    }

}
