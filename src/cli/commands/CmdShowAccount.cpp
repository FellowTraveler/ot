// Copyright (c) 2018 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "CmdShowAccount.hpp"

#include <opentxs/opentxs.hpp>

#include <cstdint>
#include <iostream>
#include <string>

namespace opentxs
{
CmdShowAccount::CmdShowAccount()
{
    command = "showaccount";
    args[0] = "--mynym <nym id>";
    args[1] = "--myacct <account id>";
    category = catAccounts;
    help = "Show myacct's stats.";
}

void CmdShowAccount::display_row(const ui::BalanceItem& row) const
{
    const auto time = std::chrono::system_clock::to_time_t(row.Timestamp());
    otOut << " " << row.Text() << " " << row.DisplayAmount() << " "
          << std::ctime(&time) << "\n " << row.Memo() << "\n";
}

std::int32_t CmdShowAccount::runWithOptions()
{

    return run(getOption("mynym"), getOption("myacct"));
}

std::int32_t CmdShowAccount::run(std::string mynym, std::string myacct)
{
    if (!checkNym("mynym", mynym)) { return -1; }

    if (!checkAccount("myacct", myacct)) { return -1; }

    const OTIdentifier nymID = Identifier::Factory(mynym);
    const OTIdentifier accountID = Identifier::Factory(myacct);
    auto& list = Opentxs::Client().UI().AccountActivity(nymID, accountID);
    otOut << "Account " << myacct << ":\n";
    dashLine();
    auto row = list.First();

    if (row->Valid()) {
        display_row(row);
        auto last = row->Last();

        while (false == last) {
            row = list.Next();
            display_row(row);
            last = row->Last();
        }
    }

    otOut << std::endl;

    return 1;
}
}  // namespace opentxs
