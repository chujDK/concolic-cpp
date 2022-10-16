#pragma once
#include <iostream>
#include <memory>

#include "z3++.h"
#include "util.h"

class Ast {
 public:
  virtual std::ostream& dump(std::ostream& o) const;
  [[nodiscard]] virtual z3::expr _z3expr() const = 0;
  virtual ~Ast()                                 = default;

 private:
};

using AstPtr = std::shared_ptr<Ast>;

class AstUnaryOp : public Ast {
 public:
  [[nodiscard]] z3::expr _z3expr() const override = 0;

  AstUnaryOp(AstPtr opr) : operand_(std::move(opr)) {}

 protected:
  AstPtr operand_;
};

class AstConst : public Ast {
 public:
  [[nodiscard]] z3::expr _z3expr() const override;

  AstConst(z3::expr const_val) : const_(std::move(const_val)) {}

 protected:
  z3::expr const_;
};

class AstConstInt : public AstConst {
 public:
  // TODO: change int to bit vector
  AstConstInt(int const_val) : AstConst(z3ctx->int_val(const_val)) {}

  static AstPtr make(int x) { return std::make_shared<AstConstInt>(x); }

 protected:
};

class AstConstBool : public AstConst {
 public:
  AstConstBool(bool b) : AstConst(z3ctx->bool_val(b)) {}

  static AstPtr make() { return make(true); }
  static AstPtr make(bool b) { return std::make_shared<AstConstBool>(b); }
};

class AstId : public Ast {
 public:
  [[nodiscard]] z3::expr _z3expr() const override;
  AstId(z3::expr id) : id_(std::move(id)) {}

 protected:
  z3::expr id_;
};

class AstInt : public AstId {
 public:
  using AstId::AstId;
  static AstPtr make(const char* var_name) {
    // TODO: change int to bit vector
    return std::make_shared<AstInt>(z3ctx->int_const(var_name));
  }
};

class AstBool : public AstId {
 public:
  static AstPtr make() {
    auto var_name = generate_uniq_varname();
    return make(var_name.c_str());
  }

  using AstId::AstId;
  static AstPtr make(const char* var_name) {
    return std::make_shared<AstBool>(z3ctx->bool_const(var_name));
  }
};

class AstBinaryOp : public Ast {
 public:
  [[nodiscard]] z3::expr _z3expr() const override = 0;

  AstBinaryOp(AstPtr lhs, AstPtr rhs)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

 protected:
  AstPtr lhs_;
  AstPtr rhs_;
};

class AstLogicAnd : public AstBinaryOp {
  // &&
 public:
  [[nodiscard]] z3::expr _z3expr() const override;
  static AstPtr make(const AstPtr& a, const AstPtr& b) {
    return std::make_shared<AstLogicAnd>(a, b);
  }

  using AstBinaryOp::AstBinaryOp;

 private:
};

class AstEq : public AstBinaryOp {
 public:
  [[nodiscard]] z3::expr _z3expr() const override;

  static AstPtr make(const AstPtr& a, const AstPtr& b) {
    return std::make_shared<AstEq>(a, b);
  }

  using AstBinaryOp::AstBinaryOp;

 protected:
};

class AstNeq : public AstBinaryOp {
 public:
  [[nodiscard]] z3::expr _z3expr() const override;

  static AstPtr make(const AstPtr& a, const AstPtr& b) {
    return std::make_shared<AstNeq>(a, b);
  }

  using AstBinaryOp::AstBinaryOp;

 protected:
};

class AstAdd : public AstBinaryOp {
 public:
  [[nodiscard]] z3::expr _z3expr() const override;

  static AstPtr make(const AstPtr& a, const AstPtr& b) {
    return std::make_shared<AstAdd>(a, b);
  }

  using AstBinaryOp::AstBinaryOp;

 protected:
};

std::ostream& operator<<(std::ostream& o, const Ast& ast);
