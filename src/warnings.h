// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2024 The Nullion Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NULLION_WARNINGS_H
#define NULLION_WARNINGS_H

#include <string>

struct bilingual_str;

void SetMiscWarning(const bilingual_str& warning);
void SetfLargeWorkInvalidChainFound(bool flag);
/** Format a string that describes several potential problems detected by the core.
 * @param[in] verbose bool
 * - if true, get all warnings separated by <hr />
 * - if false, get the most important warning
 * @returns the warning string
 */
bilingual_str GetWarnings(bool verbose);

#endif // NULLION_WARNINGS_H
