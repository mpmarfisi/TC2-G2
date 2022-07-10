"""
Fecha: 15/04/2022
Autor: Mariano Ranucci

Descripcion: Resolución del ejercicio 7 del TP1
"""

import matplotlib.pyplot as plt
import numpy as np
import scipy.signal as sig
from splane import analyze_sys, pretty_print_bicuad_omegayq

# Señal normalizada
num = np.array([1,-1])
den = np.array([1,1])

pretty_print_bicuad_omegayq(num, den)
transfer_f = sig.TransferFunction(num, den)

plt.close('all')
analyze_sys(transfer_f, 'Transferencia')

# Señal desnormalizada
# =============================================================================
# R1 = 1K
# R2 = 1K
# R3 = 1K
# C = 1uF
# W0 = 1/(R3.C) = 1/0.001
# =============================================================================
# W0 = 1/0.001

# num = np.array([1,-W0])
# den = np.array([1,W0])

# pretty_print_bicuad_omegayq(num, den)
# transfer_f = sig.TransferFunction(num, den)

# plt.close('all')
# analyze_sys(transfer_f, 'Transferencia')