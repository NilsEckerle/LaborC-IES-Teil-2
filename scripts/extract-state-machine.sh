#!/bin/bash
DIAGRAM_NAME="diagram"
regex_STATE_add_edge='STATE_add_edge\s*\(\s*([^,\s]+)\s*,\s*([^,\s]+)\s*,\s*([^)]+)\s*\)'
regex_STATE_add_edge_with_execute='STATE_add_edge_with_execute\s*\(\s*([^,\s]+)\s*,\s*([^,\s]+)\s*,\s*([^,\s]+)\s*,\s*([^)]+)\s*\)'
regex_STATE_set_parent='STATE_set_parent\s*\(\s*([^,\s]+)\s*,\s*([^)]+)\s*\)'

# Global arrays - declared outside functions
declare -a add_edge_results=()
declare -a add_edge_with_execute_results=()
declare -a set_parent_results=()

# Function to clean up parameter names
cleanup_param() {
  echo "$1" | tr -d ' ' | sed "s/condition_//g; s/USART_//g; s/execute_//g; s/LF_//g; s/robi_//g; s/lf_//g; s/CLOCK_//g"
}

extract_add_edges() {
  echo "=== Extracting STATE_add_edge matches (3 params) ==="
  local files=$(find src -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" \) ! -path "*/state_machine/*")

  while IFS= read -r line; do
    if [[ -n "$line" ]]; then
      add_edge_results+=("$line")
      # echo "$line"
    fi
  done < <(echo "$files" | xargs perl -0777 -ne "while(/$regex_STATE_add_edge/gs) { print \"\$1,\$2,\$3\n\" }")
}

extract_add_edges_with_execute() {
  echo "=== Extracting STATE_add_edge_with_execute matches (4 params) ==="
  local files=$(find src -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" \) ! -path "*/state_machine/*")

  while IFS= read -r line; do
    if [[ -n "$line" ]]; then
      add_edge_with_execute_results+=("$line")
      # echo "$line"
    fi
  done < <(echo "$files" | xargs perl -0777 -ne "while(/$regex_STATE_add_edge_with_execute/gs) { print \"\$1,\$2,\$3,\$4\n\" }")
}

extract_set_parents() {
  echo "=== Extracting STATE_set_parent matches (2 params) ==="
  local files=$(find src -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" \) ! -path "*/state_machine/*")

  while IFS= read -r line; do
    if [[ -n "$line" ]]; then
      set_parent_results+=("$line")
      # echo "$line"
    fi
  done < <(echo "$files" | xargs perl -0777 -ne "while(/$regex_STATE_set_parent/gs) { print \"\$1,\$2\n\" }")
}

process_add_edges() {
  echo "Processing add_edge results..."
  for result in "${add_edge_results[@]}"; do
    IFS=',' read -r param1 param2 param3 <<< "$result"

    param1=$(cleanup_param "$param1")
    param2=$(cleanup_param "$param2")
    param3=$(cleanup_param "$param3")

    echo "$param1 --> $param3 : $param2" >> "${DIAGRAM_NAME}.txt"
    # echo "Added: $param1 --> $param3 : $param2"
  done
}

process_add_edges_with_execute() {
  echo "Processing add_edge_with_execute results..."
  for result in "${add_edge_with_execute_results[@]}"; do
    IFS=',' read -r param1 param2 param3 param4 <<< "$result"

    param1=$(cleanup_param "$param1")
    param2=$(cleanup_param "$param2")
    param3=$(cleanup_param "$param3")
    param4=$(cleanup_param "$param4")

    echo "$param1 --> $param4 : $param2\\n'$param3'" >> "${DIAGRAM_NAME}.txt"
    # echo "Added with execute: $param1 --> $param4 : $param2 '$param3'"
  done
}

process_set_parents() {
  echo "Processing set_parent results..."
  for result in "${set_parent_results[@]}"; do
    IFS=',' read -r param1 param2 <<< "$result"

    param1=$(cleanup_param "$param1")
    param2=$(cleanup_param "$param2")

    echo "state $param2 {
      state $param1
    }" >> "${DIAGRAM_NAME}.txt"
  # echo "Added $param2, parent of $param1"
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

create_styling() {
  echo "Add styling to diagram."
  cat >> "${DIAGRAM_NAME}.txt" << EOF
' Background colors
skinparam backgroundColor #282828
skinparam handwritten false
skinparam shadowing false

' State styling
skinparam state {
    BackgroundColor #665c54
    BorderColor #ebdbb2
    FontColor #ebdbb2
    FontSize 16
    FontStyle bold
    FontName Arial
    ArrowColor #83a598
    ArrowThickness 2
  }

' Start/End state colors - multiple approaches for compatibility
skinparam stateStart {
    BackgroundColor #b8bb26
    BorderColor #98971a
  }

skinparam stateEnd {
    BackgroundColor #fb4934
    BorderColor #cc241d
  }

' Circle/dot styling (for entry/exit points)
skinparam circle {
    BackgroundColor #b8bb26
    BorderColor #98971a
    FontColor #ebdbb2
  }

' Additional circle styling attempts
skinparam circleStartRadius 8
skinparam circleEndRadius 8
skinparam stateStartColor #b8bb26
skinparam stateEndColor #fb4934

' Arrow/transition text styling
skinparam stateArrow {
    FontColor #d5c4a1
    FontSize 14
    FontStyle bold
  }

' Generic arrow styling for transitions
skinparam arrow {
    FontColor #d5c4a1
    Color #d65d0e
  }

' Fallback font color settings
skinparam defaultFontColor #d5c4a1

' Activity colors for more complex states
skinparam activity {
    BackgroundColor #504945
    BorderColor #7c6f64
    FontColor #d5c4a1
  }

' Note styling
skinparam note {
    BackgroundColor #32302f
    BorderColor #7c6f64
    FontColor #a89984
  }

' Title styling
skinparam title {
    FontColor #fabd2f
    FontSize 18
    FontStyle bold
  }

' Legend styling
skinparam legend {
    BackgroundColor #32302f
    BorderColor #7c6f64
    FontColor #bdae93
  }
EOF
}

create_entry_edge() {
  echo "Add entry edge to diagram."
  cat >> "${DIAGRAM_NAME}.txt" << EOF
[*] --> t_state_init_robi
EOF
}

main() {
  if [[ -n "$1" ]]; then
    DIAGRAM_NAME=$1
  else
    DIAGRAM_NAME="state_machine_diagram"
  fi

  # Extract data from source files
  extract_add_edges
  extract_add_edges_with_execute
  extract_set_parents

  # Create state machine diagram
  create_diagram_header
  create_styling

  create_entry_edge

  # Process all extracted data
  process_set_parents
  process_add_edges
  process_add_edges_with_execute

  # Finalize diagram
  create_diagram_footer
}

main "$@"
