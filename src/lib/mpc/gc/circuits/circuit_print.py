import graphviz

def parse_bristol_format(file_path):
    """
    Parse a Bristol format file and extract the gates and their connections.

    Args:
        file_path (str): Path to the Bristol format file.

    Returns:
        list: A list of gates, each represented as a tuple (gate_type, inputs, outputs).
    """
    gates = []
    with open(file_path, 'r') as file:
        lines = file.readlines()

        # Skip header lines if they exist (e.g., total gates and wires count)
        for line in lines:
            parts = line.strip().split()
            if len(parts) > 3:  # Indicates a gate description
                gate_type = parts[-1]
                inputs = list(map(int, parts[:-2]))
                outputs = [int(parts[-2])]
                gates.append((gate_type, inputs, outputs))
    return gates

def generate_circuit_diagram(gates, output_path='circuit_diagram'):
    """
    Generate a visual representation of the circuit using Graphviz.

    Args:
        gates (list): List of gates as tuples (gate_type, inputs, outputs).
        output_path (str): Path to save the generated circuit diagram.
    """
    graph = graphviz.Digraph(format='png', engine='dot')
    graph.attr(rankdir='LR')  # Left to right layout

    # Add nodes and edges
    for i, (gate_type, inputs, outputs) in enumerate(gates):
        gate_label = f"{gate_type}_{i}"
        graph.node(gate_label, label=gate_type, shape='circle')

        for input_wire in inputs:
            input_label = f"wire_{input_wire}"
            graph.node(input_label, label=f"{input_wire}", shape='plaintext')
            graph.edge(input_label, gate_label)

        for output_wire in outputs:
            output_label = f"wire_{output_wire}"
            graph.node(output_label, label=f"{output_wire}", shape='plaintext')
            graph.edge(gate_label, output_label)

    # Render the graph
    graph.render(output_path, view=True)
    # graph.view()

# Example Usage
if __name__ == "__main__":
    bristol_file = "test_circuit"
    gates = parse_bristol_format(bristol_file)
    generate_circuit_diagram(gates, output_path="circuit_diagram")
