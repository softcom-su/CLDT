%PYTHON% %P%\l2p.py %N%.l.json %N%.p.json

@rem 1. Последовательное расположение участков
%PYTHON% %P%\p2S.py %N%.p.json %N%.1.S

@rem 2. Перемешивание участков
%PYTHON% %P%\p2S.py %N%.p.json %N%.2.S -s

@rem Синтаксический анализ
%PYTHON% %P%\p2ps03.py %N%.p.json %N%.p2.json

@rem 3. Внедрение мертвых данных (2 раза)
%PYTHON% %P%\pdd.py %N%.p2.json %N%.p21.json 10 10
%PYTHON% %P%\pdd.py %N%.p21.json %N%.p3.json 5  5
%PYTHON% %P%\p2S.py %N%.p3.json %N%.3.S

@rem 4. Дробление участков (4 раза)
%PYTHON% %P%\p2pd.py %N%.p3.json %N%.p31.json 5
%PYTHON% %P%\p2pd.py %N%.p31.json %N%.p32.json 5
%PYTHON% %P%\p2pd.py %N%.p32.json %N%.p33.json 5
%PYTHON% %P%\p2pd.py %N%.p33.json %N%.p4.json 5
%PYTHON% %P%\p2S.py %N%.p4.json %N%.4.S

@rem 5. Запутывание CFG
%PYTHON% %P%\p2zp32.py %N%.p4.json %N%.p5.json
%PYTHON% %P%\p2S.py %N%.p5.json %N%.5.S

@rem 6. Внедрение мертвого кода
%PYTHON% %P%\p2pc.py %N%.p5.json %N%.p6.json 50 all
%PYTHON% %P%\p2S.py %N%.p6.json %N%.6.S

@rem 7. Перемещение и кодирование участков
%PYTHON% %P%\p2pm01.py %N%.p6.json %N%.p7.json 100 main CRC16_twist
%PYTHON% %P%\p2S.py %N%.p7.json %N%.7.S
