# -*- coding: utf-8 -*-
# FormatWithPython.py — 线性扫描、无正则；选区→stdout(LF且无尾随换行)；无选区→就地改写 %f
import sys, os

# 规则：
# 1) '//' 或 '/*' 之后，压成恰好 1 个空格；
# 2) 若本行内存在 '*/'，其前面压成恰好 1 个空格；
# 3) 仍按“最长 code_end + 3”对齐注释列（保持你的原策略）。
def fmt_text(text: str) -> str:
  lines = text.splitlines(True)
  recs = []
  max_code_end = -1
  for ln in lines:
    L = len(ln)
    find = ln.find
    p = -1; j = 0
    while True:
      j = find('/', j)
      if j < 0 or j + 1 >= L: break
      c = ln[j + 1]
      if c == '/' or c == '*':
        p = j; break
      j += 1
    end = L - 1
    while end >= 0 and ln[end] in '\n\r': end -= 1
    code_end = p - 1 if p >= 0 else end
    while code_end >= 0 and ln[code_end] in ' \t': code_end -= 1
    recs.append((ln, code_end, p, end))
    if code_end > max_code_end and code_end >= 0 and ln[code_end] == ';':
      max_code_end = code_end
  if max_code_end < 0: return text
  target = max_code_end + 3
  out = []
  apd = out.append
  for ln, code_end, p, end in recs:
    if p >= 0:
      tail = ln[end + 1:]
      c2 = ln[p + 1]
      k = p + 2
      while k <= end and ln[k] in ' \t': k += 1
      if c2 == '/': suffix = '// ' + ln[k:end + 1]
      else:
        q = ln.find('*/', k, end + 1)
        if q >= 0:
          t = q - 1
          while t >= k and ln[t] in ' \t': t -= 1
          suffix = '/* ' + ln[k:t + 1] + ' */' + ln[q + 2:end + 1]
        else: suffix = '/* ' + ln[k:end + 1]
      if p > 0 and 0 <= code_end < p:
        pad = target - (code_end + 1)
        if pad < 1: pad = 1
        apd(ln[:code_end + 1] + (' ' * pad) + suffix + tail)
      else: apd(suffix + tail if p == 0 else ln[:p] + suffix + tail)
    else: apd(ln)
  return ''.join(out)

# ---- 轻量 I/O ----
def _rb(path: str) -> bytes:
  with open(path, 'rb') as f: return f.read()

def read_text(path: str) -> str:
  b = _rb(path)
  try: return b.decode('utf-8')
  except UnicodeDecodeError: return b.decode(errors='ignore')

def write_text_if_changed(path: str, new_text: str) -> None:
  nb = new_text.encode('utf-8')
  try:
    if _rb(path) == nb: return
  except Exception: pass
  with open(path, 'wb') as f: f.write(nb)

# ---- 行尾：统一 LF，并移除所有尾随换行（0 个）----
def to_lf_no_trailing(s: str) -> str:
  s = s.replace('\r\n', '\n').replace('\r', '\n')  # 先 CRLF，再孤立 CR → LF
  i = len(s) - 1
  while i >= 0 and s[i] == '\n': i -= 1
  return s[:i + 1]

def main():
  # argv[1] = %s（选区临时文件），argv[2] = %f（当前文件）
  sel_path = sys.argv[1] if len(sys.argv) > 1 else ''
  file_path = sys.argv[2] if len(sys.argv) > 2 else ''
  use_sel = False  # 选区存在且非空？
  if sel_path and os.path.isfile(sel_path):
    try: use_sel = os.path.getsize(sel_path) > 0
    except Exception: use_sel = False
  if use_sel:
    raw = read_text(sel_path)
    out = to_lf_no_trailing(fmt_text(raw))  # 统一 LF + 去尾随换行
    try:  # 额外保存一份（脚本同目录，LF 且无尾随换行）
      script_dir = os.path.dirname(os.path.abspath(__file__))
      save_path = os.path.join(script_dir, 'backup_formatted_sel.c')
      with open(save_path, 'wb') as f: f.write(out.encode('utf-8'))
    except Exception: pass
    sys.stdout.write(out)  # 不要 print()，避免隐式 '\n' 追加  :contentReference[oaicite:4]{index=4}
    return
  if not file_path or file_path == '%f' or (not os.path.exists(file_path)):  # 无选区 → 就地改写整文件
    sys.stderr.write('[FormatWithPython] invalid %%f; ensure editor has focus and file is saved.\n')
    return
  write_text_if_changed(file_path, fmt_text(read_text(file_path)))

if __name__ == '__main__':
  main()
