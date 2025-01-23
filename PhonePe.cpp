#include <bits/stdc++.h>
#include <mutex>

using namespace std;

enum IssueStatus {
    NEW,
    IN_PROGRESS,
    RESOLVED
};

struct Issue {
    string id;
    string transactionId;
    string issueType;
    string subject;
    string description;
    string customerEmail;
    IssueStatus status;
    string resolution;
    Issue(string id, string transactionId, string type, string subj, string desc, string email) : id(id), transactionId(transactionId), issueType(type), subject(subj), description(desc), 
        customerEmail(email), status(NEW){}
};

struct Agent {
    string id;
    string name;
    string email;
    vector<string> expertise;
    queue<string> issueQueue;

    Agent(string id, string agentName, string email, vector<string> exp) : id(id), name(agentName), email(email), expertise(exp) {}
};

//Implemented Singleton design pattern here
class IssueManager {
private:
    unordered_map<string, Issue*> issues;
    unordered_map<string, Agent*> agents;
    unordered_map<string, vector<string>> agentWorkHistory;
    vector<string> waitlist;
    int issueCounter = 1;
    int agentCounter = 1;
    mutex lock;

    IssueManager() {}

    string generateIssueId() {
        return "I" + to_string(issueCounter++);
    }

    string generateAgentId(const string &email) {
        return "A" + to_string(agentCounter++);
    }

    string statusToString(IssueStatus status) {
        switch (status) {
            case NEW:
                return "New";
            case IN_PROGRESS:
                return "In Progress";
            case RESOLVED:
                return "Resolved";
            default:
                return "Unknown";
        }
    }
    
public:
    static IssueManager &getInstance() {
        lock_guard<mutex> lock(mutex);
        IssueManager instance;
        return instance;
    }

    string createIssue(string transactionId, string issueType, string subject, string description, string email) {
        string issueId = generateIssueId();
        issues[issueId] = new Issue(issueId, transactionId, issueType, subject, description, email);
        cout << "Issue " << issueId << " created against transaction \"" << transactionId << "\"" << endl;
        return issueId;
    }

    void addAgent(string email, string name, vector<string> expertise) {
        string agentId = generateAgentId(email);
        agents[agentId] = new Agent(agentId, name, email, expertise);
        agentWorkHistory[agentId] = {};
        cout << "Agent " << agentId << " created" << endl;
    }

    void assignIssue(string issueId) {
        if (issues.find(issueId) == issues.end()) {
            cout << "Issue " << issueId << " does not exist." << endl;
            return;
        }

        Issue* issue = issues[issueId];
        for (auto &[agentId, agent] : agents) {
            if (find(agent->expertise.begin(), agent->expertise.end(), issue->issueType) != agent->expertise.end()) {
                if (agent->issueQueue.empty()) {
                    agent->issueQueue.push(issueId);
                    cout << "Issue " << issueId << " assigned to agent " << agentId << endl;
                    return;
                }
            }
        }
        waitlist.push_back(issueId);
        cout << "Issue " << issueId << " added to waitlist" << endl;
    }
    //first 

    void getIssues(const string &filterKey, const string &filterValue) {
        for (const auto &[issueId, issue] : issues) {
            if ((filterKey == "email" && issue->customerEmail == filterValue) ||
                (filterKey == "type" && issue->issueType == filterValue)) {
                cout << issueId << " {" << issue->transactionId << ", " << issue->issueType << ", " << issue->subject
                     << ", " << issue->description << ", " << issue->customerEmail << ", " << issue->status << "}" << endl;
            }
        }
    }

    void updateIssue(string issueId, IssueStatus status, string resolution) {
        if (issues.find(issueId) == issues.end()) {
            cout << "Issue " << issueId << " does not exist." << endl;
            return;
        }

        issues[issueId]->status = status;
        issues[issueId]->resolution = resolution;
        cout << "Issue " << issueId << " status updated to " << statusToString(status) << endl;
    }

    void resolveIssue(const string &issueId, const string &resolution) {
        if (issues.find(issueId) == issues.end()) {
            cout << "Issue " << issueId << " does not exist." << endl;
            return;
        }

        issues[issueId]->status = RESOLVED;
        issues[issueId]->resolution = resolution;

        for (auto &[agentId, agent] : agents) {
            if (!agent->issueQueue.empty() && agent->issueQueue.front() == issueId) {
                agent->issueQueue.pop();
                agentWorkHistory[agentId].push_back(issueId);
                break;
            }
        }
        cout << "Issue " << issueId << " marked resolved" << endl;
    }

    void viewAgentsWorkHistory() {
        for (auto &[agentId, workHistory] : agentWorkHistory) {
            cout << agentId << "-> {";
            for (const auto &issueId : workHistory) {
                cout << issueId << ", ";
            }
            cout << "}" << endl;
        }
    }
};

/*
enum ISSUE_FILTER{
    EMAIL, NAME, TYPE
};
FilterStrategy {

    filter()
}

// {Agent, number}
AssignStrategy {

    assign() {

    }

    getVariance() {

    }
}

RandomAssignStrategy extends AssignStrategy {
    unordered_map<agendId, cnt> 
    assign() {

    }
}

SequentialAssignStrategy extends AssignStrategy {

}


AssignStrategyController {
    AssignStrategy assignStrategy;
    setStrategy(AssignStrategy strategy) {
        assignStrategy = strategy;
    }

}
*/

int main() {
    IssueManager &manager = IssueManager::getInstance();

    manager.createIssue("T1", "Payment Related", "Payment Failed", "My payment failed but money is debited", "testUser1@test.com");
    manager.createIssue("T2", "Mutual Fund Related", "Purchase Failed", "Unable to purchase Mutual Fund", "testUser2@test.com");
    manager.createIssue("T3", "Payment Related", "Payment Failed", "My payment failed but money is debited", "testUser2@test.com");

    manager.addAgent("agent1@test.com", "Agent 1", {"Payment Related", "Gold Related"});
    manager.addAgent("agent2@test.com", "Agent 2", {"Payment Related"});

    manager.assignIssue("I1");
    manager.assignIssue("I2");
    manager.assignIssue("I3");

    manager.getIssues("email", "testUser2@test.com");
    manager.getIssues("type", "Payment Related");

    manager.resolveIssue("I1", "PaymentFailed debited amount will get reversed");

    manager.updateIssue("I3", IN_PROGRESS, "Waiting for payment confirmation");
    
    manager.resolveIssue("I3", "PaymentFailed debited amount will get reversed");

    manager.viewAgentsWorkHistory();
    return 0;
}
