# 📱 C++ Social Network Simulator

This is a terminal-based simulation of a social media platform built in C++. It supports features like user creation, following, posting, liking, commenting, direct messaging, mutual friend detection, trending post display, and shortest path search between users (BFS on the follow graph).

---

## 🧠 Features

- ✅ Create user accounts  
- ✅ Follow/unfollow other users  
- ✅ Create and manage posts  
- ✅ Like and comment on posts  
- ✅ Send and view private messages  
- ✅ Find mutual friends  
- ✅ Find shortest connection path  
- ✅ View trending posts (by likes)  
- ✅ Display user profile and activity  

---

## 🏗️ Architecture Overview

### Post
- Contains: post ID, content, likes, comments
- Methods: `addLike()`, `addComment()`

### User
- Contains: username, followers, followings, posts, messages
- Methods: `follow()`, `addPost()`, `likePost()`, `commentOnPost()`, `sendMessage()`, `viewMessagesFrom()`

### SocialNetwork
- Manages all users and interactions
- Features: Add user, follow user, create post, like/comment, send/view messages, mutual friends, shortest path, trending posts

---
### Menu Option
1. Add User
2. Follow User
3. Post
4. Like Post
5. Comment
6. Send Message
7. View Messages
8. Shortest Path
9. Mutual Friends
10. Trending Posts
11. Show User Info
12. Exit


## 🛠️ How to Compile & Run

```bash
g++ -std=c++17 -o social_network main.cpp
./social_network
