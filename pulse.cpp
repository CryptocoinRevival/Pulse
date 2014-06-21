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
	int nHeight = prevBlock->nHeight+1;
	printf("Checking Block %u for Pulse: ", nHeight);
	if( PULSE_RATE > 0 && block->GetBlockTime() > prevBlock->GetBlockTime()+PULSE_RATE ) {
		printf("+Rate ");
		ret = true;
	} //else {
		if( PULSE_MIN_TX > 0 && block->vtx.size() > PULSE_MIN_TX ) {
			printf("+Min_TX ");
			ret = true;
		}
		if( PULSE_MIN_VALUE > 0 || PULSE_MIN_FEE > 0 ) {
			int64 value = 0;
			int64 fee = 0;
			CCoinsViewCache cc(*pcoinsTip, true);
			BOOST_FOREACH(const CTransaction& tx, block->vtx) {
				value += tx.GetValueOut();
				if( PULSE_MIN_FEE > 0 )
					fee += tx.GetValueIn(cc);
			}
			value = value - GetBlockValue(nHeight, nFees, blockIndex.nBits);
			fee = fee - value;
			printf("Value: %llu - Fee: %llu - Reward: %llu ", value, fee, GetBlockValue(nHeight, nFees, blockIndex.nBits));
			if( PULSE_MIN_VALUE > 0 && value >= (PULSE_MIN_VALUE * COIN) ) {
				printf("+Min_Value ");
				ret = true;
			}
			if( PULSE_MIN_FEE > 0   && fee   >= (PULSE_MIN_FEE * COIN) ) {
				printf("+Min_Fee ");
				ret = true;
			}
		}
	//}
	if( ret )
		printf(": Accepted\n");
	else
		printf(": Rejected\n");
	return ret;
}
