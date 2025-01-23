/*
Round-1 (Machine Coding): 

Design a PhonePe Resolution Service.

A scenario like there are tickets/issues and there are agents and application 
will either assign the ticket to an agent or it will be added to the backlog of an agent. 
For matching a ticket with the appropriate agent, 
the application can consider the ticket-type and match it with an agent with that speciality.
If no agent with that speciality is found, the ticket can be assigned to any agent or its backlog.

Experience: Had 2.5 hrs to code it on my personal IDE and then send it back to the recruiter over email (after zipping it).
The code was fairly straightforward and be sure to use strategy pattern for the ticket assignment process, 
so that the assignment logic is pluggable. 
All in-memory data structures were used.

[Word of caution]: Time Management!!

Evaluation of the round happened a couple of days later. It was a 30-min discussion 
where I had to discuss about the code and explain the design and implementation choices.

Entitities -> Ticket/Issue, Speciality
Actors - Agent, System, User
IssueResolutionService - , AssignIssue, 
CreateTicket - User
ResolveTicket -> resolved by agent
*/

#include<bits/stdc++.h>

using namespace std;

enum IssueType {
    PAYMENT, INVESTMENT, ACCOUNT
};

enum IssueStatus {
    CREATED, IN_PROGRESS, COMPLETED
};

class Issue {
private:
    string id;
    IssueType type;
    string desciption;
    IssueStatus status;
    string agentId;
public:
    Issue(string id, IssueType issueType, string description) {
        this->id = id;
        this->type = issueType;
        this->desciption = desciption;
        this->agentId = "";
        this->status = IssueStatus::COMPLETED;
    }

    IssueType getIssueType() {
        return type;
    }

    string getAgentId() {
        return agentId;
    }

    void setAgentId(string agentId) {
        this->agentId = agentId;
    }
};

class Account {
private:
    string id;
    string name;
    string password;
public:
    Account(string id, string name, string password) {
        this->id = id;
        this->name = name;
        this->password = password;
    }
};

class Agent: public Account {
private:
    set<IssueType> specialities;
    vector<Issue> issues;
    vector<Issue> backlogs;
public:
    Agent(string id, string name, string password, set<IssueType> specialities): Account(id, name, password), specialities(specialities) {}
    
    set<IssueType> getSpecialities() {
        return specialities;
    }

    void addIssue(Issue issue) {
        issues.push_back(issue);
    }

    int getIssueSize() {
        return issues.size();
    }

    void addBackLog(Issue issue) {
        backlogs.push_back(issue);
    }
};

class AgentService {
private:
    unordered_map<string, Agent*> agents;
    int cnt = 0;
public:
    Agent* createAgent() {
        cnt++;
        string id = to_string(cnt);
        Agent* agent = new Agent(id, "agentName", "123", {IssueType::ACCOUNT, IssueType::PAYMENT});
        agents[id] = agent;
        return agent;
    }

    string findAgent(IssueType issueType) {
        string agentId;
        for(auto agent: agents) {
            Agent* agentptr = agent.second;
            agentId = agent.first;
            set<IssueType> specialities = agentptr->getSpecialities();
            if(specialities.find(issueType) != specialities.end()) {
                return agent.first;
            }
        }
        return agentId;
    }

    void addIssueToAgent(string agentId, Issue issue) {
        Agent* agent = agents[agentId];
        if(agent->getIssueSize() <= 1) agent->addIssue(issue);
        else agent->addBackLog(issue);
    }
};


class IssueService {
private:
    unordered_map<string, Issue*> issues, assineeIssueIdMapping;
    AgentService* agentService;
    int cnt = 0;
public:
    IssueService(AgentService* agentService): agentService(agentService){}

    Issue* createIssue(IssueType issueType, string description) {
        ++cnt;
        string id = to_string(cnt);
        Issue* issue = new Issue(id, issueType, description);
        issues[id] = issue;
        assisgnIssue(issue);
        return issue;
    }

    void assisgnIssue(Issue* issue) {
        string agentId = agentService->findAgent(issue->getIssueType());
        issue->setAgentId(agentId);
        agentService->addIssueToAgent(agentId, *issue);
    }
};



class User: public Account {
private:
    vector<Issue> issues;
public:
};

class UserService {
private:
    unordered_map<string, User> users; //{userId, User}
    IssueService issueService;
public:
    UserService(IssueService issueService): issueService(issueService) {}
    Issue* createIssue(string description, IssueType issueType) {
        return issueService.createIssue(issueType, description);
    }
};
/*
interface Command{
    execute();
}

CreateIssueCommand extend Command{
    IssueService issueService;
    execute() {
        issueService.createIssue();
    };
}

MarkResolveIssueCommand extend Command{
    IssueService issueService;
    execute() {
        issueService.markAsResolved();
    };
}
*/

int main() {
    AgentService* agentService = new AgentService();
    IssueService* issueService = new IssueService(agentService);
    UserService* userService = new UserService(*issueService);
    Agent* agent1 = agentService->createAgent();
    Issue* issue = userService->createIssue("My payment issue", IssueType::PAYMENT);
    cout << agent1->getIssueSize() << "\n";
    cout << issue->getAgentId();
    return 0;
}