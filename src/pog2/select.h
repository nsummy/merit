// Copyright (c) 2018 The Merit Foundation developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MERIT_POG2_SELECT_H
#define MERIT_POG2_SELECT_H

#include "hash.h"
#include "amount.h"
#include "pog2/cgs.h"
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <memory>

#include <map>

namespace referral
{
    class ReferralsViewCache;
}

namespace pog2
{
    using InvertedEntrants = pog2::Entrants;
    using AddressToEntrant = std::map<referral::Address, pog2::Entrant>;
    using SampledAddresses = std::set<referral::Address>;

    class CgsDistribution
    {
        public:
            CgsDistribution(pog2::Entrants entrants);
            const pog2::Entrant& Sample(const uint256& hash) const;
            size_t Size() const;

        private:
            InvertedEntrants m_inverted;
            AddressToEntrant m_cgses;
            SampledAddresses m_sampled;
            CAmount m_max_cgs = 0;
    };

    using CgsDistributionPtr = std::unique_ptr<CgsDistribution>;

    class AddressSelector
    {
        public:
            AddressSelector(int height, const pog2::Entrants& entrants);

            pog2::Entrants SelectOld(
                    const referral::ReferralsViewCache& referrals,
                    uint256 hash,
                    size_t n) const;

            pog2::Entrants SelectNew(
                    const referral::ReferralsViewCache& referrals,
                    uint256 hash,
                    size_t n) const;

            size_t Size() const;
        private:
            pog2::Entrants Select(
                    const referral::ReferralsViewCache& referrals,
                    uint256 hash,
                    size_t n,
                    const CgsDistribution& distribution) const;

            CgsDistributionPtr m_old_distribution;
            CgsDistributionPtr m_new_distribution;
    };

    referral::ConfirmedAddresses SelectConfirmedAddresses(
            const referral::ReferralsViewDB& db,
            uint256 hash,
            const uint160& genesis_address,
            size_t n,
            std::set<referral::Address> &unconfirmed_invites,
            int max_outstanding_invites);

    /**
     * Returns true if the address type is valid for ambassador lottery.
     */
    bool IsValidAmbassadorDestination(char type);
} // namespace pog2

#endif //MERIT_POG2_SELECT_H