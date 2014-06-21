// Copyright (c) 2014-.... f0o / The Cryptocoin Revival Foundation
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef PULSE_H
#define PULSE_H

class CBlock;
class CBlockIndex;
class CCoinsView;
class CCoinsViewCache;
static int64 nFees;
static int64 GetBlockValue(int nHeight, int64 nFees, unsigned int nBits);
extern CCoinsViewCache *pcoinsTip;

static const          int PULSE_HEIGHT    = 1;   /** Height to start Pulse */
static const          int PULSE_RATE      = 300; /** Rate to Pulse in seconds */
static const          int PULSE_REWARD    = 2;   /** Reward of Pulse-Blocks (without COIN base) */
static const unsigned int PULSE_MIN_TX    = 10;   /** Minimum amount (not value of!) of TX in a block to bypass Pulse-Rate */
static const          int PULSE_MIN_VALUE = 10;  /** Minimum value of the TX in a block to bypass Pulse-Rate (without COIN base) */
static const          int PULSE_MIN_FEE   = 2;   /** Minimum value of accumulated fees of the TX in a block to bypass Pulse-Rate (without COIN base) */

static bool Pulse(int nHeight);
static bool Pulse(CBlockIndex* prevBlock, CBlock* block);

#endif
