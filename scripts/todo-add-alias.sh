#!/bin/bash

# TODO management aliases - source this file to activate
# Usage: source todo-add-alias.sh

alias todo="nvim TODO.md"
alias todos="grep -n '\- \[ \]' TODO.md"
alias todone="grep -n '\- \[x\]' TODO.md"
alias branches="git branch | grep -E '(issue|feature|bug|refactor)/'"

echo "TODO aliases activated!"
echo "Available commands:"
echo "  todo    - Edit TODO.md"
echo "  todos   - Show open tasks"
echo "  todone  - Show completed tasks"
echo "  branches - Show feature/issue branches"
