// Copyright (c) 2014-.... f0o / The Cryptocoin Revival Foundation
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#include "pulse.h"

static bool Pulse(int nHeight) {
	printf("Checking for Pulse on %u: ",nHeight);
	if( nHeight >= PULSE_HEIGHT ) {
		printf("True\n");
		return true;
	}
	printf("False\n");
	return false;
}

//Goes in CBlock::AcceptBlock
static bool Pulse(CBlockIndex* prevBlock, CBlock* block) {
	bool ret = false;

	CBlockHeader blockHead = block->GetBlockHeader();
	CBlockIndex blockIndex = CBlockIndex(blockHead);
	CCoinsViewCache view(*pcoinsTip, true);
	int nHeight = prevBlock->nHeight+1;
	int rate = block->GetBlockTime() - prevBlock->GetBlockTime();
	if( PULSE_RATE > 0 && rate > PULSE_RATE ) {
		ret = true;
	} else if( PULSE_MIN_RATE > 0 && rate < PULSE_MIN_RATE ) {
		ret = false;
	} else {
		if( PULSE_MIN_TX > 0 && block->vtx.size() > PULSE_MIN_TX ) {
			ret = true;
		} else if( PULSE_MIN_VALUE > 0 || PULSE_MIN_FEE > 0 ) {
			int64 value = 0;
			int64 fee = 0;
			BOOST_FOREACH(const CTransaction& tx, block->vtx) {
				if( PULSE_MIN_FEE > 0 && value > 0 ) {
					BOOST_FOREACH(const CTxIn txin, tx.vin)
						if( !view.HaveCoins(txin.prevout.hash) ) {
							return false;
						}
					fee += tx.GetValueIn(view) - tx.GetValueOut();
				}
				value += tx.GetValueOut();
			}
			value = value - GetBlockValue(nHeight, fee, blockIndex.nBits);
			if( PULSE_MIN_VALUE > 0 && value >= (PULSE_MIN_VALUE * COIN) ) {
				ret = true;
			} else if( PULSE_MIN_FEE > 0   &&  fee  >= ( PULSE_MIN_FEE  * COIN) ) {
				ret = true;
			}
		}
	}
	return ret;
}
