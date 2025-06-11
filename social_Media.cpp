#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

class Post {
public:
    int postId;
    string content;
    vector<string> comments;
    int likes = 0;

    Post(int id, const string& text) : postId(id), content(text) {}

    void addComment(const string& comment) {
        comments.push_back(comment);
    }

    void addLike() {
        likes++;
    }
};

class User {
public:
    string username;
    unordered_set<User*> followers;
    unordered_set<User*> following;
    unordered_map<int, Post*> posts;
    unordered_map<string, vector<string>> messages;

    User(string name) : username(name) {}

    void follow(User* user) {
        following.insert(user);
        user->followers.insert(this);
    }

    void addPost(int postId, const string& content) {
        posts[postId] = new Post(postId, content);
    }

    void likePost(User* other, int postId) {
        if (other->posts.count(postId)) {
            other->posts[postId]->addLike();
        }
    }

    void commentOnPost(User* other, int postId, const string& comment) {
        if (other->posts.count(postId)) {
            other->posts[postId]->addComment(comment);
        }
    }

    void sendMessage(const string& toUser, const string& msg) {
        messages[toUser].push_back(msg);
    }

    void viewMessagesFrom(const string& fromUser) {
        if (messages.count(fromUser)) {
            cout << "Messages from " << fromUser << ":\n";
            for (auto& msg : messages[fromUser]) {
                cout << " - " << msg << "\n";
            }
        } else {
            cout << "No messages from " << fromUser << endl;
        }
    }
};

class SocialNetwork {
private:
    unordered_map<string, User*> users;

public:
    void addUser(const string& name) {
        if (!users.count(name))
            users[name] = new User(name);
        else
            cout << "User already exists.\n";
    }

    void followUser(const string& a, const string& b) {
        if (users.count(a) && users.count(b)) {
            users[a]->follow(users[b]);
        }
    }

    void createPost(const string& user, int postId, const string& content) {
        if (users.count(user))
            users[user]->addPost(postId, content);
    }

    void likePost(const string& user, const string& author, int postId) {
        if (users.count(user) && users.count(author))
            users[user]->likePost(users[author], postId);
    }

    void commentOnPost(const string& user, const string& author, int postId, const string& comment) {
        if (users.count(user) && users.count(author))
            users[user]->commentOnPost(users[author], postId, comment);
    }

    void sendMessage(const string& from, const string& to, const string& msg) {
        if (users.count(from) && users.count(to)) {
            users[from]->sendMessage(to, msg);
        }
    }

    void viewMessages(const string& user, const string& from) {
        if (users.count(user))
            users[user]->viewMessagesFrom(from);
    }

    vector<string> findShortestPath(const string& start, const string& end) {
        if (!users.count(start) || !users.count(end)) return {};

        unordered_map<string, string> parent;
        queue<string> q;
        unordered_set<string> visited;
        vector<string> path;

        q.push(start);
        visited.insert(start);

        while (!q.empty()) {
            string curr = q.front(); q.pop();
            if (curr == end) {
                while (curr != start) {
                    path.push_back(curr);
                    curr = parent[curr];
                }
                path.push_back(start);
                reverse(path.begin(), path.end());
                return path;
            }

            for (auto neighbor : users[curr]->following) {
                string name = neighbor->username;
                if (!visited.count(name)) {
                    visited.insert(name);
                    parent[name] = curr;
                    q.push(name);
                }
            }
        }
        return {};
    }

    vector<string> findMutualFriends(const string& u1, const string& u2) {
        if (!users.count(u1) || !users.count(u2)) return {};

        unordered_set<string> mutual;
        for (auto f1 : users[u1]->following) {
            for (auto f2 : users[u2]->following) {
                if (f1 == f2) {
                    mutual.insert(f1->username);
                }
            }
        }
        return vector<string>(mutual.begin(), mutual.end());
    }

    void showTrendingPosts() {
        vector<pair<int, string>> allPosts;

        for (auto& user : users) {
            for (auto& pair : user.second->posts) {
                allPosts.push_back({ pair.second->likes, pair.second->content });
            }
        }

        sort(allPosts.rbegin(), allPosts.rend());

        cout << "\n🔥 Trending Posts:\n";
        for (int i = 0; i < min(3, (int)allPosts.size()); i++) {
            cout << " - \"" << allPosts[i].second << "\" [" << allPosts[i].first << " likes]\n";
        }
    }

    void showUserInfo(const string& username) {
        if (!users.count(username)) return;
        User* user = users[username];
        cout << "User: " << user->username << "\nFollowers: ";
        for (auto f : user->followers) cout << f->username << " ";
        cout << "\nFollowing: ";
        for (auto f : user->following) cout << f->username << " ";
        cout << "\nPosts:\n";
        for (auto& pairs : user->posts) {
            cout << "[" << pairs.first << "] " << pairs.second->content << " (" << pairs.second->likes << " likes)\n";
            for (auto& c : pairs.second->comments) cout << "   💬 " << c << "\n";
        }
        cout << endl;
    }
};

int main() {
    SocialNetwork sn;
    int choice;

    while (true) {
        cout << "\n1. Add User\n2. Follow User\n3. Post\n4. Like Post\n5. Comment\n6. Send Message\n7. View Messages\n";
        cout << "8. Shortest Path\n9. Mutual Friends\n10. Trending Posts\n11. Show User Info\n12. Exit\nEnter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string name;
            cout << "Enter username: ";
            getline(cin, name);
            sn.addUser(name);
        } else if (choice == 2) {
            string f1, f2;
            cout << "Follower: "; getline(cin, f1);
            cout << "Followed: "; getline(cin, f2);
            sn.followUser(f1, f2);
        } else if (choice == 3) {
            string name, content;
            int id;
            cout << "Username: "; getline(cin, name);
            cout << "Post ID: "; cin >> id; cin.ignore();
            cout << "Content: "; getline(cin, content);
            sn.createPost(name, id, content);
        } else if (choice == 4) {
            string name, author;
            int id;
            cout << "Username: "; getline(cin, name);
            cout << "Post Author: "; getline(cin, author);
            cout << "Post ID: "; cin >> id; cin.ignore();
            sn.likePost(name, author, id);
        } else if (choice == 5) {
            string name, author, comment;
            int id;
            cout << "Username: "; getline(cin, name);
            cout << "Post Author: "; getline(cin, author);
            cout << "Post ID: "; cin >> id; cin.ignore();
            cout << "Comment: "; getline(cin, comment);
            sn.commentOnPost(name, author, id, comment);
        } else if (choice == 6) {
            string from, to, msg;
            cout << "From: "; getline(cin, from);
            cout << "To: "; getline(cin, to);
            cout << "Message: "; getline(cin, msg);
            sn.sendMessage(from, to, msg);
        } else if (choice == 7) {
            string user, from;
            cout << "Your username: "; getline(cin, user);
            cout << "Messages from: "; getline(cin, from);
            sn.viewMessages(user, from);
        } else if (choice == 8) {
            string from, to;
            cout << "From: "; getline(cin, from);
            cout << "To: "; getline(cin, to);
            auto path = sn.findShortestPath(from, to);
            if (path.empty()) cout << "No path found.\n";
            else {
                cout << "Path: ";
                for (auto& p : path) cout << p << " ";
                cout << endl;
            }
        } else if (choice == 9) {
            string u1, u2;
            cout << "User 1: "; getline(cin, u1);
            cout << "User 2: "; getline(cin, u2);
            auto mutual = sn.findMutualFriends(u1, u2);
            if (mutual.empty()) cout << "No mutual friends.\n";
            else {
                cout << "Mutual Friends: ";
                for (auto& m : mutual) cout << m << " ";
                cout << endl;
            }
        } else if (choice == 10) {
            sn.showTrendingPosts();
        } else if (choice == 11) {
            string user;
            cout << "Username: "; getline(cin, user);
            sn.showUserInfo(user);
        } else if (choice == 12) {
            break;
        }
    }

    return 0;
}
