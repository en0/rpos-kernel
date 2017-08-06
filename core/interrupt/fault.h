/**
 ** Copyright (c) 2017 "Ian Laird"
 ** Research Project Operating System (rpos) - https://github.com/en0/rpos
 ** 
 ** This file is part of rpos
 ** 
 ** rpos is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 ** 
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 ** 
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef _INTERRUPT_FAULT_H
#define _INTERRUPT_FAULT_H 1

#define FAULT_DE 0
#define FAULT_DB 1
#define FAULT_NMI 2
#define FAULT_BP 3
#define FAULT_OF 4
#define FAULT_BR 5
#define FAULT_UD 6
#define FAULT_NM 7
#define FAULT_DF 8
#define FAULT_TS 10
#define FAULT_NP 11
#define FAULT_SS 12
#define FAULT_GP 13
#define FAULT_PF 14
#define FAULT_MF 16
#define FAULT_AC 17
#define FAULT_MC 18
#define FAULT_XM 19
#define FAULT_XF 19
#define FAULT_VE 20
#define FAULT_SX 30

/*
 * fault_stub*(fault_number, error_code)
 * This is defined in fault.s. 
 *
 * fault_stub* is the gate entry for the given fault code where * represents 
 * the fault number.
 *
 * Note: This should not be called directly. It is called by the processor when
 *       a fault is raised.
 *
 * Arguments:
 * - fautl_number: a integer value identifing the fault number that was raised.
 * - error_code: Aditional inforamation about the fault (if applicable).
 *
 * Returns:
 * - None
 */

void fault_stub0(void);
void fault_stub1(void);
void fault_stub2(void);
void fault_stub3(void);
void fault_stub4(void);
void fault_stub5(void);
void fault_stub6(void);
void fault_stub7(void);
void fault_stub8(void);
void fault_stub10(void);
void fault_stub11(void);
void fault_stub12(void);
void fault_stub13(void);
void fault_stub14(void);
void fault_stub16(void);
void fault_stub17(void);
void fault_stub18(void);
void fault_stub19(void);
void fault_stub20(void);
void fault_stub30(void);

/* 
 * initialize_api()
 * Setup fault gates and install default exception hander. Additionally,
 * provide an interface for other codepages to install specific exception
 * handlers such as page_fault.
 *
 * Arguments:
 * - None
 *
 * Returns:
 * - None
 */

void fault_initialize_api();

/*
 * install_fault_handler(num, fn)
 * Install a function handler for the given fault number.
 *
 * Arguments:
 * - num: An integer that indicates a fault number.
 * - fn: A function with the signature fn(regs_t*).
 *
 * Returns:
 * - None
 */

void install_fault_handler(int fault_number, void(*fn)(regs_t*));

#endif /** __FAULT_H **/
