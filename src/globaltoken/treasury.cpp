// Copyright (c) 2020 The Globaltoken Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <globaltoken/treasury.h>
#include <uint256.h>
#include <hash.h>
#include <script/script.h>

CTreasuryMempool activeTreasury;

bool CTreasuryProposal::IsNull() const
{
    return (*this == CTreasuryProposal());
}

bool CTreasuryProposal::IsHeadlineValid() const
{
    return strHeadline.length() <= MAX_HEADLINE_LENGTH;
}

bool CTreasuryProposal::IsDescriptionValid() const
{
    return strDescription.length() <= MAX_DESCRIPTION_LENGTH;
}

bool CTreasuryProposal::IsExpired(const uint32_t nSystemTime) const
{
    return nSystemTime >= nExpireTime;
}

bool CTreasuryProposal::IsAgreed() const
{
    return fAgreed;
}

bool CTreasuryProposal::SetAgreed()
{
    if(fAgreed)
        return false; // Already agreed
    
    fAgreed = true;
    return true;
}

bool CTreasuryProposal::UnsetAgreed()
{
    if(!fAgreed)
        return false; // Already not agreed
    
    fAgreed = false;
    return true;
}

uint256 CTreasuryProposal::GetHash() const
{
    return SerializeHash(*this);
}

void CTreasuryMempool::SetTreasuryDir (const std::string &dir)
{
    strTreasuryDir = dir;
}

void CTreasuryMempool::SetTreasuryFile (const std::string &file)
{
    strTreasuryFile = file;
}

std::string CTreasuryMempool::GetTreasuryDir () const
{
    return strTreasuryDir;
}

std::string CTreasuryMempool::GetTreasuryFile () const
{
    return strTreasuryFile;
}

bool CTreasuryMempool::IsCached() const
{
    return (nVersion != 0);
}

void CTreasuryMempool::SetVersion (const uint32_t nNewVersion)
{
    nVersion = nNewVersion;
}

void CTreasuryMempool::SetLastSaved (const uint32_t nNewLastSaved)
{
    nLastSaved = nNewLastSaved;
}

uint32_t CTreasuryMempool::GetVersion() const
{
    return nVersion;
}

uint32_t CTreasuryMempool::GetLastSaved() const
{
    return nLastSaved;
}

uint256 CTreasuryMempool::GetHash() const
{
    return SerializeHash(*this);
}

void CTreasuryMempool::DeleteExpiredProposals(const uint32_t nSystemTime)
{
    for(size_t i = 0; i < vTreasuryProposals.size(); i++)
    {
        if(vTreasuryProposals[i].IsExpired(nSystemTime))
            vTreasuryProposals.erase(vTreasuryProposals.begin() + i);
    }
}

bool CTreasuryMempool::SearchScriptByScript(const CScript &script, size_t &nIndex) const
{
    for(size_t i = 0; i < vRedeemScripts.size(); i++)
    {
        if(vRedeemScripts[i] == script)
        {
            nIndex = i;
            return true;
        }
    }
    return false;
}

bool CTreasuryMempool::RemoveScriptByID(const size_t nIndex)
{
    if(nIndex >= vRedeemScripts.size())
        return false;
    
    vRedeemScripts.erase(vRedeemScripts.begin() + nIndex);
    return true;
}

bool CTreasuryMempool::GetProposalvID(const uint256& hash, size_t& nIndex) const
{
    for(size_t i = 0; i < activeTreasury.vTreasuryProposals.size(); i++)
    {
        if(activeTreasury.vTreasuryProposals[i].hashID == hash)
        {
            nIndex = i;
            return true;
        }
    }
    return false;
}