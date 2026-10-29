#!/usr/bin/env python3
"""Прогоняет настоящие бинарники и складывает реальный вывод в assets/term.json.

Отчёт рисует из этого JSON терминальные окна вектором (функция terminal()
в template/oaip.typ) — растр не нужен. Приглашения и наборы ввода заданы
явно: если приглашение не найдено или программа вернула неожидаемый код
возврата, генерация падает, чтобы рисунки не врали.

Каждая ячейка матрицы читается отдельной строкой, поэтому приглашений
"a[i][j] = " ровно n*m — они и перечислены в SESSIONS.
"""
import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent  # home/
ASSETS = ROOT / "report" / "assets"
TITLE = "sehaxe@cachyos: ~/OAiP-Laba4/home"


def cells(n: int, m: int) -> list[str]:
    return [f"a[{i}][{j}] = " for i in range(n) for j in range(m)]


SIZE = "Размеры матрицы n m (1..100): "
MODE = "Заполнение: 1 - вручную, 2 - случайно: "
SIZE_N = "Размер матрицы n (1..100): "

# (приглашения по порядку, строки ввода по порядку) для каждого запуска.
SESSIONS = {
    1: [
        (
            [SIZE, SIZE, MODE] + cells(3, 4),
            ["abc", "3 4", "1", "1", "2", "3", "4", "5", "0", "6",
             "7", "8", "9", "0", "10"],
        ),
        ([SIZE, MODE], ["3 3", "2"]),
    ],
    2: [
        (
            [SIZE, MODE] + cells(4, 3),
            ["4 3", "1", "-1", "-2", "-3", "4", "5", "-6", "0", "1",
             "2", "-7", "-8", "-9"],
        ),
        (
            [SIZE, MODE] + cells(2, 2),
            ["2 2", "1", "7", "8", "-5", "-6"],
        ),
    ],
    3: [
        (
            [SIZE_N, MODE] + cells(5, 5),
            ["5", "1", "1", "2", "3", "4", "5", "6", "7", "8", "9",
             "10", "11", "12", "13", "14", "15", "16", "17", "18",
             "19", "20", "21", "22", "23", "24", "25"],
        ),
        ([SIZE_N, MODE], ["6", "2"]),
    ],
}


def screen_lines(prompts: list[str], user_input: str, out: str) -> list[dict]:
    """Экран сессии: после каждого приглашения — эхо набранного ввода."""
    inputs = user_input.split("\n") if user_input else []
    if len(inputs) != len(prompts):
        raise RuntimeError(f"вводов {len(inputs)}, приглашений {len(prompts)}")
    lines: list[dict] = []
    pos = 0
    for prompt, answer in zip(prompts, inputs):
        idx = out.find(prompt, pos)
        if idx < 0:
            raise RuntimeError(f"приглашение не найдено: {prompt!r}")
        for text in out[pos:idx].splitlines():
            if text:
                lines.append({"out": text})
        lines.append({"echo": {"prompt": prompt, "input": answer}})
        pos = idx + len(prompt)
    for text in out[pos:].splitlines():
        if text:
            lines.append({"out": text})
    return lines


# Что должно быть в выводе каждого запуска: страховка, что текст отчёта
# не разошёлся с программой (числа в «ходе решения» взяты из этих запусков).
EXPECTED = {
    (1, 0): "Столбцов без нулевых элементов: 2",
    (1, 1): "Столбцов без нулевых элементов: 3",
    (2, 0): "Первая строка с положительным элементом: 2 (индекс 1)",
    (2, 1): "Предыдущей строки нет",
    (3, 0): "Максимальный элемент в области 1: 8",
    (3, 1): "Максимальный элемент в области 1: 44",
}


def run_task(n: int, runs: list[tuple[list[str], list[str]]]) -> list[dict]:
    session = []
    for k, (prompts, inputs) in enumerate(runs):
        user_input = "\n".join(inputs)
        proc = subprocess.run(
            [f"./{n}/main"],
            input=user_input + "\n",
            capture_output=True,
            text=True,
            cwd=ROOT,
        )
        if proc.returncode != 0:
            raise RuntimeError(f"task {n}: код возврата {proc.returncode}")
        want = EXPECTED[(n, k)]
        if want not in proc.stdout:
            raise RuntimeError(f"task {n}, запуск {k}: нет {want!r}")
        session.append({
            "cmd": f"./{n}/main",
            "lines": screen_lines(prompts, user_input, proc.stdout),
        })
    return session


if __name__ == "__main__":
    ASSETS.mkdir(parents=True, exist_ok=True)
    data = {}
    for n, runs in SESSIONS.items():
        data[str(n)] = {"title": TITLE, "runs": run_task(n, runs)}
        print(f"ok: задача {n} ({len(runs)} запуска)")
    out = ASSETS / "term.json"
    out.write_text(json.dumps(data, ensure_ascii=False, indent=2), encoding="utf-8")
    print(f"ok: {out.relative_to(ROOT)}")
