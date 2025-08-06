#!/bin/bash
DIAGRAM_NAME="state_machine_diagram"
regex_STATE_add_edge='STATE_add_edge\s*\(\s*([^,\s]+)\s*,\s*([^,\s]+)\s*,\s*([^)]+)\s*\)'
regex_STATE_add_edge_with_execute='STATE_add_edge_with_execute\s*\(\s*([^,\s]+)\s*,\s*([^,\s]+)\s*,\s*([^,\s]+)\s*,\s*([^)]+)\s*\)'
regex_STATE_set_parent='STATE_set_parent\s*\(\s*([^,\s]+)\s*,\s*([^)]+)\s*\)'

# Global arrays - declared outside functions
declare -a add_edge_results=()
declare -a add_edge_with_execute_results=()
declare -a set_parent_results=()

extract_add_edges() {
  echo "=== Extracting STATE_add_edge matches (3 params) ==="
  local files=$(find src -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" \) ! -path "*/state_machine/*")
  
  while IFS= read -r line; do
    if [[ -n "$line" ]]; then
      add_edge_results+=("$line")
      echo "$line"
    fi
  done < <(echo "$files" | xargs perl -0777 -ne "while(/$regex_STATE_add_edge/gs) { print \"\$1,\$2,\$3\n\" }")
}

extract_add_edges_with_execute() {
  echo "=== Extracting STATE_add_edge_with_execute matches (4 params) ==="
  local files=$(find src -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" \) ! -path "*/state_machine/*")
  
  while IFS= read -r line; do
    if [[ -n "$line" ]]; then
      add_edge_with_execute_results+=("$line")
      echo "$line"
    fi
  done < <(echo "$files" | xargs perl -0777 -ne "while(/$regex_STATE_add_edge_with_execute/gs) { print \"\$1,\$2,\$3,\$4\n\" }")
}

extract_set_parents() {
  echo "=== Extracting STATE_set_parent matches (2 params) ==="
  local files=$(find src -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" \) ! -path "*/state_machine/*")
  
  while IFS= read -r line; do
    if [[ -n "$line" ]]; then
      set_parent_results+=("$line")
      echo "$line"
    fi
  done < <(echo "$files" | xargs perl -0777 -ne "while(/$regex_STATE_set_parent/gs) { print \"\$1,\$2\n\" }")
}

process_add_edges() {
  echo "Processing add_edge results..."
  for result in "${add_edge_results[@]}"; do
    IFS=',' read -r param1 param2 param3 <<< "$result"
    # Remove any whitespace
    param1=$(echo "$param1" | tr -d ' ' | sed "s/condition_//g; s/USART_//g; s/execute_//g; s/LF_//g")
    param2=$(echo "$param2" | tr -d ' ' | sed "s/condition_//g; s/USART_//g; s/execute_//g; s/LF_//g")
    param3=$(echo "$param3" | tr -d ' ' | sed "s/condition_//g; s/USART_//g; s/execute_//g; s/LF_//g")
    
    echo "$param1 --> $param3 : $param2" >> "${DIAGRAM_NAME}.txt"
    echo "Added: $param1 --> $param3 : $param2"
  done
}

process_add_edges_with_execute() {
  echo "Processing add_edge_with_execute results..."
  for result in "${add_edge_with_execute_results[@]}"; do
    IFS=',' read -r param1 param2 param3 param4 <<< "$result"
    param1=$(echo "$param1" | tr -d ' ' | sed "s/condition_//g; s/USART_//g; s/execute_//g; s/LF_//g")
    param2=$(echo "$param2" | tr -d ' ' | sed "s/condition_//g; s/USART_//g; s/execute_//g; s/LF_//g")
    param3=$(echo "$param3" | tr -d ' ' | sed "s/condition_//g; s/USART_//g; s/execute_//g; s/LF_//g")
    param4=$(echo "$param4" | tr -d ' ' | sed "s/condition_//g; s/USART_//g; s/execute_//g; s/LF_//g")
    
    echo "$param1 --> $param4 : $param2\\n'$param3'" >> "${DIAGRAM_NAME}.txt"
    echo "Added with execute: $param1 --> $param4 : $param2 '$param3'"
  done
}

process_set_parents() {
  echo "Processing set_parent results..."
  for result in "${set_parent_results[@]}"; do
    IFS=',' read -r param1 param2 <<< "$result"
    param1=$(echo "$param1" | tr -d ' ' | sed "s/condition_//g; s/USART_//g; s/execute_//g; s/LF_//g")
    param2=$(echo "$param2" | tr -d ' ' | sed "s/condition_//g; s/USART_//g; s/execute_//g; s/LF_//g")
    
    echo "state $param2 {
    state $param1
  }" >> "${DIAGRAM_NAME}.txt"
    echo "Added $param2, parent of $param1"
  done
}

create_diagram_header() {
  echo "=== Generating $DIAGRAM_NAME ==="
  cat > "${DIAGRAM_NAME}.txt" << EOF
@startuml
EOF
}

create_diagram_footer() {
  cat >> "${DIAGRAM_NAME}.txt" << EOF
@enduml
EOF
  echo "State machine diagram written to ${DIAGRAM_NAME}.txt"
}

create_entry_edge() {
  cat >> "${DIAGRAM_NAME}.txt" << EOF
[*] --> t_state_init_robi
EOF
}

main() {
  # Extract data from source files
  extract_add_edges
  extract_add_edges_with_execute
  extract_set_parents
  
  # Create state machine diagram
  create_diagram_header

  create_entry_edge
  
  # Process all extracted data
  process_set_parents
  process_add_edges
  process_add_edges_with_execute
  
  # Finalize diagram
  create_diagram_footer
}

main "$@"
