import numpy as np
from qiskit import QuantumCircuit

qc = QuantumCircuit(3)
qc.h(0)
qc.p(np.pi / 2, 0)
qc.cx(0, 1)
qc.cx(0, 2)

qc_measured = qc.measure_all(inplace=False)

from qiskit.primitives import StatevectorSampler
sampler = StatevectorSampler()
job = sampler.run([qc_measured], shots=1000)
result = job.result()
print(f" > Counts: {result[0].data['meas'].get_counts()}")

from qiskit.quantum_info import SparsePauliOp
operator = SparsePauliOp.from_list([("XXY", 1), ("XYX", 1), ("YXX", 1), ("YYY", -1)])

from qiskit.primitives import StatevectorEstimator
estimator = StatevectorEstimator()
job = estimator.run([(qc, operator)], precision=1e-3)
result = job.result()
print(f" > Expectation values: {result[0].data.evs}")

from qiskit import transpile
from qiskit.transpiler import Target, CouplingMap
target = Target.from_configuration(
    basis_gates= ["cz", "rz", "sx"],
    coupling_map=CouplingMap.from_line(4),
)
qc_transpiled = transpile(qc, target=target)

print(f" > Transpiled: {qc_transpiled}")

#Toffoli
from qiskit import QuantumCircuit

def Toffoli_manual(qc):
    qc.h(2)      #Hadamard
    qc.cx(1, 2)  #CNOT
    qc.tdg(2)    #inverse pi/8 transform
    qc.cx(0, 2)  #CNOT
    qc.t(2)      #pi/8 transform
    qc.cx(1, 2)  #CNOT
    qc.tdg(2)    #inverse pi/8 transform
    qc.cx(0, 2)  #CNOT
    qc.t(1)      #pi/8 transform
    qc.t(2)      #pi/8 transform
    qc.h(2)      #Hadamard
    qc.cx(0, 1)  #CNOT
    qc.t(0)      #pi/8 transform
    qc.tdg(1)    #inverse pi/8 transform
    qc.cx(0, 1)  #CNOT
    # 6 CNOT, 3 pi/8, 3 inverse pi/8, 2 Hadamard

print("Truth table (Toffoli):")

for i in range(8):
    qc = QuantumCircuit(3)

    bits = format(i, '03b')  # e.g. '101'
    for qubit, bit in enumerate(bits):
        if bit == '1':
            qc.x(qubit)

    Toffoli_manual(qc)
    qc.measure_all()

    job = sampler.run([qc], shots=100)
    result = job.result()
    counts = result[0].data['meas'].get_counts()
    output = max(counts, key=counts.get)  # most frequent result

    print(f"{bits} -> {output}")

print("\n")
#Margolus
def Margolus_manual(qc):
    qc.h(2)      #Hadamard
    qc.cx(1, 2)  #CNOT
    qc.tdg(2)    #inverse pi/8 transform
    qc.cx(0, 2)  #CNOT
    qc.t(2)      #pi/8 transform
    qc.cx(1, 2)  #CNOT
    qc.tdg(2)    #inverse pi/8 transform
    qc.h(2)      #Hadamard

    # 3 CNOT, 1 pi/8, 2 inverse pi/8, 2 Hadamard

print("Truth table (Margolus):")
for i in range(8):
    qc = QuantumCircuit(3)

    bits = format(i, '03b')
    for qubit, bit in enumerate(bits):
        if bit == '1':
            qc.x(qubit)

    Margolus_manual(qc)
    qc.measure_all()

    job = sampler.run([qc], shots=100)
    result = job.result()
    counts = result[0].data['meas'].get_counts()
    output = max(counts, key=counts.get)

    print(f"{bits} -> {output}")

print("\n")

#Adder
from qiskit import QuantumCircuit
def ripple_adder(n):
    qc = QuantumCircuit(3*n + 1)
    a = list(range(n))
    b = list(range(n, 2*n))
    c = list(range(2*n, 3*n + 1))

    for i in range(n):
        #Carry cycle
        qc.ccx(a[i], b[i], c[i+1])
        qc.cx(a[i], b[i])
        qc.ccx(b[i], c[i], c[i+1])

    qc.cx(c[n-1], b[n-1])

    for i in range(n-2, -1, -1):
        #Carry
        qc.ccx(b[i], c[i], c[i+1])
        qc.cx(a[i], b[i])
        qc.ccx(a[i], b[i], c[i+1])
        #Sum
        qc.cx(c[i], b[i])
        qc.cx(a[i], b[i])

    return qc, a, b, c

n = 4
adder_qc, a, b, c = ripple_adder(n)
init_qc = QuantumCircuit(3*n + 1)

# A = 1111
init_qc.x(a[0])
init_qc.x(a[1])
init_qc.x(a[2])
init_qc.x(a[3])
# B = 1111
init_qc.x(b[0])
init_qc.x(b[1])
init_qc.x(b[2])
init_qc.x(b[3])

qc = init_qc.compose(adder_qc)
qc.measure_all()

from qiskit.primitives import StatevectorSampler
sampler = StatevectorSampler()
result = sampler.run([qc], shots=1000).result()
counts = result[0].data['meas'].get_counts()

for bitstring, count in counts.items():
    rev = bitstring[::-1]
    result_bits = ''.join(rev[i] for i in b) + rev[c[-1]]
    print(f"Result: {result_bits}") #reverse order
