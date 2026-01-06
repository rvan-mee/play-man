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

#pragma once

#include <play-man/gameboy/cartridge/Cartridge.hpp>
#include <play-man/gameboy/memory/MemoryBus.hpp>
#include <play-man/gameboy/opcodes/Opcodes.hpp>
#include <play-man/gameboy/cpu/Instruction.hpp>
#include <play-man/containers/EnumIndexableArray.hpp>

#include <functional>
#include <stdint.h>

namespace GameBoy
{	
    class Cpu
    {
        friend struct TestFixtures::GameBoyCpuFixture;

        using InstructionPrototype = Instruction::InstructionPrototype; /*!< -. */
    
    private:
        std::shared_ptr<ACartridge>     cartridge;
        CpuCore                         core;
        MemoryBus                       memoryBus;

        size_t cycles = 0;
        Instruction currentInstruction; /*< The current instruction to execute/is being executed. */

        static constexpr size_t numberOfInstructions = 256;
        static constexpr size_t numberOfPrefixedInstructions = 256;

        EnumIndexableArray<OpCode, InstructionPrototype, numberOfInstructions> instructions;
        EnumIndexableArray<PrefixedOpCode, InstructionPrototype, numberOfPrefixedInstructions> prefixedInstructions;

    public:

        Cpu() = delete;
        Cpu(std::shared_ptr<ACartridge> _cartridge) : cartridge(_cartridge), memoryBus(cartridge, core)
        {
            InitInstructionTable();
        };

        /**
         * @brief Used for testing, overwrites the current ROM data with the data
         *        found inside the file at filePath. Also resets all the internal registers.
         * @param filePath The location of the test rom.
         */
        void LoadTestRom(const char *filePath);

        /**
         * @brief -.
         * @param opCode 
         */
        void ExecuteInstruction(OpCode opCode);

        /**
         * @brief -.
         * @param opCode 
         */
        void ExecuteInstruction(PrefixedOpCode opCode);

        /**
         * @brief Logs the current instruction being executed in debug mode.
         */
        void LogInstruction();

        /**
         * @brief Executes the current instruction.
         */
        void ExecuteInstruction();

        /**
         * @brief Fetches the instruction located on the address stored inside the PC register.
         */
        void FetchInstruction();

        /*!
         * @brief Reads byte from memory bus at address.
         * @brief address
         * @return
        */
        uint8_t Fetch(uint16_t address);

        /**
         * @brief Reads byte from memory bus at address contained in PC; increments PC;
         * @return 
         */
        uint8_t FetchPcAddress();

        /**
         * @brief Reads two bytes from the memory bus at address contained in PC; increments PC by 2;
         * 
         * @note The first byte will be the upper byte of the final 16 bit value.
         * 
         * @return The 16bit data found at the address contained in PC.
         */
        uint16_t FetchPcAddress16bit();

        /**
         * @brief Initializes the instruction array.
         */
        void InitInstructionTable();

//////////////////
// Instructions //
//////////////////
private:

        /**
         * @brief Invalid OpCodes result in the CPU hard-locking itself until powered off
         */
        size_t HardLock();

        /**
		 * @brief
		 * @return number of cycles.
		 */
        size_t NOP();

        /**
         * @brief Adjusts the A register to a binary-coded decimal number.
         * 
         * @note binary-coded decimal number utilizes 4 bits for the numbers 0 through 9.
         *       This allows the 8 bit register to go from 0 to 99, using the lower half
         *       of the register for the x9 numbers and the top half 9x numbers.
         * 
         * @note https://blog.ollien.com/posts/gb-daa/
         * 
         * @return number of cycles.
         */
        size_t DDA();

        /**
         * @brief Applies the one's complement on register A.
         *        (flips all the bits).
         * 
         * @return number of cycles.
         */
        size_t CPL();

        /**
         * @brief Sets the carry flag inside the F register.
         * 
         * @note Also sets the SUB and HALF_CARRY flags to false.
         * 
         * @return number of cycles.
         */
        size_t SCF();

        /**
         * @brief Flips the carry flag.
         * 
         * @note Also sets the SUB and HALF_CARRY flags to false.
         * 
         * @return number of cycles.
         */
        size_t CCF();

        /**                               Jmp/call instructions                                             **/

        /**
         * @brief Loads two bytes of immediate date into the PC reg.
         *        First byte of immediate data is low byte.
         * @return number of cycles.
         */
        size_t Jump_16bit_ImmediateData();

        /**
         * @brief Increments the PC register by the 8 bit value of the immediate data.
         * @return number of cycles.
         */
        size_t Jump_Relative_8bit_SignedImmediateData();

        /**
         * @brief Check if the flag is set, if it is not, increments the PC register
         *        by the signed 8 bit value of the immediate data.
         * @return number of cycles.
         */
	    size_t Jump_Relative_FlagNotSet_8bit_SignedImmediateData(FlagRegisterFlag flag);

        /**
         * @brief Check if the flag is set, if it is, increments the PC register
         *        by the signed 8 bit value of the immediate data.
         * @return number of cycles.
         */
        size_t Jump_Relative_FlagSet_8bit_SignedImmediateData(FlagRegisterFlag flag);

        /**                                16 bit instructions                                             **/

		/**
		 * @brief Loads two bytes of immediate data into reg.
		 * 		  First byte of immediate data is low byte.
		 * @param reg 
		 * @return size_t 
		 */
		size_t Load_16bit_ImmediateData(Register CpuCore::* reg);

        /**
         * @brief Increments the 16 bit register by 1.
         * 
         * @param reg Pointer to the register needing to be incremented.
         * @return number of cycles.
         */
        size_t Increment_16bit(Register CpuCore::* reg);

        /**
         * @brief Decrements the 16 bit register by 1.
         * 
         * @param reg Pointer to the register needing to be decremented.
         * @return number of cycles.
         */
        size_t Decrement_16bit(Register CpuCore::* reg);

        /**
         * @brief Loads the contents of the 16bit register to the address found at PC.
         * 
         * @param reg Pointer to the register where the data is taken from.
         * 
         * @return number of cycles.
         */
        size_t Load_16bit_RegToImmediateAddr(Register CpuCore::* reg);

        /**
         * @brief Adds the contents from fromReg to the toReg.
         * 
         * @note Sets the S flag to false.
         * @note Sets the H flag according to the calculation.
         * @note Sets the C flag according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Add_16bit(Register CpuCore::* toReg, Register CpuCore::* fromReg);


        /**                                8 bit instructions                                              **/

        /**
         * @brief Increments the high part of the register by 1.
         * 
         * @param reg Pointer to the register needing to be incremented.
         * 
         * @note The Z flag is set if the incrementation results in a 0.
         * @note The S flag is set to false.
         * @note The H flag is set according to the calculation.
         * 
         * @return number of cycles. 
         */
        size_t Increment_8bit_High(Register CpuCore:: *reg);

        /**
         * @brief Increments the low part of the register by 1.
         * 
         * @param reg Pointer to the register needing to be incremented.
         * 
         * @note The Z flag is set if the incrementation results in a 0.
         * @note The S flag is set to false.
         * @note The H flag is set according to the calculation.
         * 
         * @return number of cycles. 
         */
        size_t Increment_8bit_Low(Register CpuCore:: *reg);

        /**
         * @brief Increments the value pointed to by addrReg by one.
         * 
         * @param addrReg Pointer the the register containing the address of the value
         *                that needs to be incremented.
         * 
         * @note The Z flag is set if the increment results in a 0.
         * @note the S flag is set to false.
         * @note the H flag is set according to the calculation.
         */
        size_t Increment_Dereferenced(Register CpuCore:: *addrReg);

        /**
         * @brief Decrements the high part of the register by 1.
         * 
         * @param reg Pointer to the register needing to be decremented.
         * 
         * @note The Z flag is set if the decrementation results in a 0.
         * @note The S flag is set to true.
         * @note The H flag is set according to the calculation.
         * 
         * @return number of cycles. 
         */
        size_t Decrement_8bit_High(Register CpuCore:: *reg);

        /**
         * @brief Decrements the low part of the register by 1.
         * 
         * @param reg Pointer to the register needing to be decremented.
         * 
         * @note The Z flag is set if the decrementation results in a 0.
         * @note The S flag is set to true.
         * @note The H flag is set according to the calculation.
         * 
         * @return number of cycles. 
         */
        size_t Decrement_8bit_Low(Register CpuCore:: *reg);

        /**
         * @brief Decrements the value pointed to by addrReg by one.
         * 
         * @param addrReg Pointer the the register containing the address of the value
         *                that needs to be decremented.
         * 
         * @note The Z flag is set if the decrement results in a 0.
         * @note the S flag is set to true.
         * @note the H flag is set according to the calculation.
         */
        size_t Decrement_Dereferenced(Register CpuCore::* addrReg);

        /**
         * @brief Stores the byte found in the high register of dataReg to the addres
         *        Pointed to by addrReg.
         * 
         * @param addrReg Pointer to the register containing the address where the data needs to be stored. 
         * @param dataReg Pointer to the register where the data will be taken from (high 8 bits).
         * 
         * @return number of cycles.
         */
        size_t Store_8bit_Addr_High(Register CpuCore::* addrReg, Register CpuCore::* dataReg);

        /**
         * @brief Stores the byte found in the low register of dataReg to the addres
         *        pointed to by addrReg.
         * 
         * @param addrReg Pointer to the register containing the address where the data needs to be stored. 
         * @param dataReg Pointer to the register where the data will be taken from (low 8 bits).
         * 
         * @return number of cycles.
         */
        size_t Store_8bit_Addr_Low(Register CpuCore::* addrReg, Register CpuCore::* dataReg);

        /**
         * @brief Stores the byte found in the high register of dataReg to the addres
         *        pointed to by addrReg after which the contents of addrReg gets incremented by 1.
         * 
         * @param addrReg Pointer to the register containing the address where the data needs to be stored. 
         * @param dataReg Pointer to the register where the data will be taken from (high 8 bits).
         * 
         * @return number of cycles.
         */
        size_t Store_8bit_AddrIncrement_High(Register CpuCore::* addrReg, Register CpuCore::* dataReg);

        /**
         * @brief Stores the byte found in the high register of dataReg to the addres
         *        pointed to by addrReg after which the contents of addrReg gets decremented by 1.
         * 
         * @param addrReg Pointer to the register containing the address where the data needs to be stored. 
         * @param dataReg Pointer to the register where the data will be taken from (high 8 bits).
         * 
         * @return number of cycles.
         */
        size_t Store_8bit_AddrDecrement_High(Register CpuCore::* addrReg, Register CpuCore::* dataReg);

        /**
         * @brief Stores the byte found at the current PC register location to the destination
         *        pointed to by addrReg.
         * 
         * @param addrReg pointer to the register containing the address where the data needs to be stored.
         * 
         * @return number of cycles.
         */
        size_t Store_8bit_Addr_ImmediateData(Register CpuCore::* addrReg);

        /**
         * @brief Loads the 8bit immediate data into the high register of reg.
         * 
         * @param reg Pointer to the register where the data will be loaded into (high 8 bits).
         * 
         * @return number of cycles. 
         */
        size_t Load_8bit_High_ImmediateData(Register CpuCore::* reg);

        /**
         * @brief Loads the 8bit immediate data into the low register of reg.
         * 
         * @param reg Pointer to the register where the data will be loaded into (low 8 bits).
         * 
         * @return number of cycles. 
         */
        size_t Load_8bit_Low_ImmediateData(Register CpuCore::* reg);

        /**
         * @brief Loads the 8bits of data found at the addrReg into destReg.
         * 
         * @param destReg Pointer to the register where the data is loaded into the low 8 bits.
         * @param addrReg Pointer to the register containing the address where the data is located.
         * 
         * @return number of cycles. 
         */
        size_t Load_8bit_Low_Addr(Register CpuCore::* destReg, Register CpuCore::* addrReg);

        /**
         * @brief Loads the 8bits of data found at the addrReg into destReg.
         * 
         * @param destReg Pointer to the register where the data is loaded into the high 8 bits.
         * @param addrReg Pointer to the register containing the address where the data is located.
         * 
         * @return number of cycles. 
         */
        size_t Load_8bit_High_Addr(Register CpuCore::* destReg, Register CpuCore::* addrReg);

        /**
         * @brief Loads the 8 bits of data found at addrReg into destReg, after which the pointer inside
         *        addrReg gets incremented by 1.
         * 
         * @param destReg Pointer to the register where the data is loaded into the high 8 bits.
         * @param addrReg Pointer to the register containing the address where the data is located.
         *                Will get incremented by 1 after the data is retrieved.
         * 
         * @return number of cycles.
         */
        size_t Load_8bit_High_AddrIncrement(Register CpuCore::* destReg, Register CpuCore::* addrReg);

        /**
         * @brief Loads the 8 bits of data found at addrReg into destReg, after which the pointer inside
         *        addrReg gets decremented by 1.
         * 
         * @param destReg Pointer to the register where the data is loaded into the high 8 bits.
         * @param addrReg Pointer to the register containing the address where the data is located.
         *                Will get decremented by 1 after the data is retrieved.
         * 
         * @return number of cycles.
         */
        size_t Load_8bit_High_AddrDecrement(Register CpuCore::* destReg, Register CpuCore::* addrReg);

        /**
         * @brief Loads the contents of the high part of fromReg into the high part of toReg.
         * 
         * @param toReg Register where the high part the value will be loaded into.
         * @param fromReg Register where the high part's value will be loaded from.
         * 
         * @return number of cycles.
         */
        size_t Load_8bit_High_High(Register CpuCore::* toReg, Register CpuCore::* fromReg);

        /**
         * @brief Loads the contents of the low part of fromReg into the high part of toReg.
         * 
         * @param toReg Register where the high part the value will be loaded into.
         * @param fromReg Register where the low part's value will be loaded from.
         * 
         * @return number of cycles.
         */
        size_t Load_8bit_High_Low(Register CpuCore::* toReg, Register CpuCore::* fromReg);

        /**
         * @brief Loads the contents of the high part of fromReg into the low part of toReg.
         * 
         * @param toReg Register where the low part the value will be loaded into.
         * @param fromReg Register where the high part's value will be loaded from.
         * 
         * @return number of cycles.
         */
        size_t Load_8bit_Low_High(Register CpuCore::* toReg, Register CpuCore::* fromReg);

        /**
         * @brief Loads the contents of the low part of fromReg into the low part of toReg.
         * 
         * @param toReg Register where the low part the value will be loaded into.
         * @param fromReg Register where the low part's value will be loaded from.
         * 
         * @return number of cycles.
         */
        size_t Load_8bit_Low_Low(Register CpuCore::* toReg, Register CpuCore::* fromReg);

        /**
         * @brief Compares the 8 bit immediate data to the high register by calculating (reg - data).
         * 
         * @note The Z flag is set if they are equal.
         * @note The S flag is set to true.
         * @note The H flag is set according to the calculation.
         * @note The C flag is set according to the calculation.
         * 
         * @return number of cycles.
         */
        size_t Compare_8bit_High_ImmediateData(Register CpuCore::* reg);

        /**
         * @brief Rotates the given register's high part to the left by 1,
         *        appending the shifted-out bit to the right of that same register.
         * 
         * @note The Z flag is set to false.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t Rotate_8bit_High_Left(Register CpuCore::* reg);

        /**
         * @brief Rotats the given register's high part to the left by 1,
         *        appending the state of the carry flag bit to the right of the same register.
         *        The shifted-out bit will become the new state of the carry flag.
         * 
         * @note The Z flag is set to false.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t Rotate_8bit_High_Left_Carry(Register CpuCore::* reg);

        /**
         * @brief Rotates the given register's low part to the right by 1,
         *        appending the shifted-out bit to the left of that same register.
         * 
         * @note The Z flag is set to false.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t Rotate_8bit_Low_Right(Register CpuCore::* reg);

        /**
         * @brief Rotates the given register's high part to the right by 1,
         *        appending the state of the carry flag bit to the left of the same register. 
         *        The shifted-out bit will become the new state of the carry flag.
         * 
         * @note The Z flag is set to false.
         * @note The S flag is set to false.
         * @note The H flag is set to false.
         * @note The C flag is set to the shifted-out bit.
         * 
         * @return number of cycles.
         */
        size_t Rotate_8bit_High_Right_Carry(Register CpuCore::* reg);
    };

}
