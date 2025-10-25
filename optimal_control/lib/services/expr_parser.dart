import '../store.dart';
Map<String, String>? make_term_map() {
  try {
    int dim = int.parse(AppStore().dim);
    Map<String, String> res = {};
    for (int i = 0; i < dim; i++) {
      String n = (i + 1).toString();
      res["x" + n] = "x_{" + n + "}";
      res["u" + n] = "u_{" + n + "}";
      res["psi" + n] = "\\psi_{" + n + "}";
    }
    return res;
  } catch(e) {
    return null;
  }
}
class ExprParser {
  final Map<String, String>? _terms;
  ExprParser(): _terms =  make_term_map();
  static bool _isDelim(String c) => c == ' ';
  static bool _isOp(String c) => ['+', '-', '*', '/', '^'].contains(c);
  static bool _isFunction(String s) => ['sin', 'cos'].contains(s);
  static int _priority(String op) {
    if (op.codeUnitAt(0) < 0) return 4;
    return switch(op) {
      '+' || '-' => 1,
      '*' || '/' => 2,
      '^' => 3,
      _ => -1
    };
  }
  static void _processedOp(List<String> st, String op) {
    if (op.startsWith('u') || op.startsWith('f')) {
      String l = st.removeLast();
      switch(op) {
        case 'u+': st.add(l); break;
        case 'u-': st.add('-' + l); break;
        case 'fs': st.add('\\sin(' + l + ')'); break;
        case 'fc': st.add('\\cos(' + l + ')'); break;
      }
    }
    else {
      String r = st.removeLast();
      String l = st.removeLast();
      switch(op) {
        case '+': st.add(l + '+' + r); break;
        case '-': st.add(l + '-' + r); break;
        case '*': st.add(l + '*' + r); break;
        case '/': st.add('\\frac{' + l + '}{' + r + '}'); break;
        case '^': st.add(l + '^{' + r + '}'); break;
      }
    }
  }
  String parse(String s) {
    if (_terms != null){
      bool mayUnary = true;
      List<String> st = [];
      List<String> op = [];
      for (int i = 0; i < s.length; i++) {
        String char = s[i];
        if (!_isDelim(char)) {
          if (char == '(') {
            op.add('(');
            mayUnary = true;
          } else if (char == ')') {
            while(op.isNotEmpty && op.last != '(') {
              _processedOp(st, op.removeLast());
            }
            op.removeLast();
            mayUnary = false;
            String cur_expr = st.removeLast();
            st.add('($cur_expr)');
          } else if (_isOp(char)) {
            String curOp = char;
            if (mayUnary && (curOp == '+' || curOp == '-')){
              curOp = curOp == '+' ? 'u+' : 'u-';
            }
            while(op.isNotEmpty && op.last != '(' && _priority(op.last) >= _priority(curOp))
              _processedOp(st,op.removeLast());
            op.add(curOp);
            mayUnary = true;
          } else {
            String token = '';
            while( i < s.length && s[i].contains(RegExp(r'[a-zA-Z0-9.]'))) {
              token += s[i++];
            }
            i--;
            if (_terms.containsKey(token)) {
              st.add(_terms[token] ?? '');
              mayUnary = false;
            } else if (double.tryParse(token) != null) {
              st.add(token);
              mayUnary = false;
            } else if (_isFunction(token)) {
              String funcCode = switch(token) {
                'sin' => 'fs',
                'cos' => 'fc',
                _=>token
              };
              op.add(funcCode);
              mayUnary = true;
            }
          }
        }
      }
      while(op.isNotEmpty) {
        _processedOp(st,op.removeLast());
      }
      return st.last;
    }
    else 
      return '';
  }
}
