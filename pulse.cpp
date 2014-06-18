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

static bool Pulse(CBlockIndex* block) {
	return Pulse(block->nHeight);
}

//Goes in CBlock::AcceptBlock
static bool Pulse(CBlockIndex* prevBlock, CBlock* block) {
	bool ret = true;
	CBlockHeader blockHead = block->GetBlockHeader();
	CBlockIndex blockIndex = CBlockIndex(blockHead);
	printf("Checking Block %u for Pulse: ", blockIndex.nHeight);
	if( prevBlock->nHeight+1 >= PULSE_HEIGHT ) {
		if( PULSE_RATE > 0 && block->GetBlockTime() < prevBlock->GetBlockTime()+PULSE_RATE ) {
			printf("-Rate ");
			ret = false;
		}
		if( PULSE_MIN_TX > 0 && block->vtx.size() < PULSE_MIN_TX ) {
			printf("-Min_TX ");
			ret = false;
		}
		if( PULSE_MIN_VALUE > 0 ) {
			int64 value;
			BOOST_FOREACH(const CTransaction& tx, block->vtx)
				value += tx.GetValueOut();
			if( value < PULSE_MIN_VALUE ) {
				printf("-Min_Value ");
				ret = false;
			}
/*			if( PULSE_MIN_FEE > 1 && fees < PULSE_MIN_FEE ) {
				printf("-Min_Fee ");
				ret = false;
			} */
		}
	}
	if( ret )
		printf(": Accepted\n");
	else
		printf(": Rejected\n");
	return ret;
}

//Goes in CTxMemPool::accept
static bool Pulse(CValidationState &state, CTransaction &tx) {
	bool ret = true;
	printf("Checking TX %s for Pulse: ", tx.GetHash());
	if( ret )
		printf(": Accepted\n");
	else
		printf(": Rejected\n");
	return ret;
}
