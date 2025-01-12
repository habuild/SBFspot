/************************************************************************************************
    SBFspot - Yet another tool to read power production of SMA solar inverters
    (c)2012-2018, SBF
    
    Latest version found at https://github.com/SBFspot/SBFspot
    
    License: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
    http://creativecommons.org/licenses/by-nc-sa/3.0/
    
    You are free:
        to Share - to copy, distribute and transmit the work
        to Remix - to adapt the work
    Under the following conditions:
    Attribution:
        You must attribute the work in the manner specified by the author or licensor
        (but not in any way that suggests that they endorse you or your use of the work).
    Noncommercial:
        You may not use this work for commercial purposes.
    Share Alike:
        If you alter, transform, or build upon this work, you may distribute the resulting work
        only under the same or similar license to this one.

DISCLAIMER:
    A user of SBFspot software acknowledges that he or she is receiving this
    software on an "as is" basis and the user is not relying on the accuracy
    or functionality of the software for any purpose. The user further
    acknowledges that any use of this software will be at his own risk
    and the copyright owner accepts no responsibility whatsoever arising from
    the use or application of the software.
    
    SMA is a registered trademark of SMA Solar Technology AG

************************************************************************************************/

#include "EventData.h"
#include "TagDefs.h"
#include <boost/algorithm/string/replace.hpp>
#include <sstream>
#include <iomanip>

extern TagDefs tagdefs;

unsigned int EventData::GroupTagID() const
{
    const unsigned int GroupDefOffset = 829;
    uint32_t GroupDef = m_Group & 0x1F;
    if ((GroupDef > 0) && (GroupDef <= 17))
        return GroupDef + GroupDefOffset;   // 830 = LriGrpStt (Status)
    else
        return 0;
}

unsigned int EventData::UserGroupTagID() const
{
    if (m_UserGroup == 0x07)    // UG_USER
        return 861; // Usr
    else if (m_UserGroup == 0x0A) //UG_INSTALLER
        return 862; // Istl
    else
        return 0;   // Should never happen
}

std::string EventData::EventType() const
{
    switch (m_EventFlags & 3)
    {
    case 0: return "Incoming";
    case 1: return "Outgoing";
    case 2: return "Event";
    case 3: return "Acknowledge";
    case 4: return "Reminder";
    case 7: return "Invalid";
    default: return "N/A";
    }
}

std::string EventData::EventCategory() const
{
    switch ((m_EventFlags >> 14) & 3)
    {
    case 0: return "Info";
    case 1: return "Warning";
    case 2: return "Error";
    default: return "None";
    }
}

std::string EventData::EventDescription() const
{
    std::string eventDescr = tagdefs.getDesc(Tag());

    if (eventDescr.find("|") != std::string::npos)
    {
        boost::replace_all(eventDescr, "|ln04|", quote(tagdefs.getDescForLRI(Parameter()).c_str()));
        boost::replace_all(eventDescr, "|tn0|", tagdefs.getDesc(DT_Change()).c_str());
        boost::replace_all(eventDescr, "|tn4|", tagdefs.getDesc(Parameter()).c_str());
        boost::replace_all(eventDescr, "|tn8|", quote(tagdefs.getDesc(NewVal()).c_str()));
        boost::replace_all(eventDescr, "|tnc|", quote(tagdefs.getDesc(OldVal()).c_str()));
        // |s0|
        // |d0|
        // |u0|
        // |x0||x1||x2|...|x9|
    }

    return eventDescr;
}

std::string EventData::ToString() const
{
    std::stringstream ss;
    ss << ToLocalTime(DateTime(), "%Y%m%d@%H%M%S") << " " << EventDescription();

    return ss.str();
}

std::string EventData::ToString(const char *datetimeformat) const
{
    std::stringstream ss;
    ss << ToLocalTime(DateTime(), datetimeformat) << " " << EventDescription();

    return ss.str();
}

std::string EventData::ToLocalTime(const time_t rawtime, const char *format) const
{
    struct tm* timeinfo = localtime(&rawtime);
    std::ostringstream os;
    os << std::put_time(timeinfo, format);
    return os.str();
}

