#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

namespace chess {

// Error types for chess operations
enum class ChessError {
    InvalidFen,
    InvalidMove,
    InvalidPosition,
    ParseError,
    OutOfBounds,
    IllegalMove,
    GameOver
};

// Convert error to string for debugging
[[nodiscard]] constexpr std::string_view errorToString(ChessError error) noexcept {
    switch (error) {
        case ChessError::InvalidFen:
            return "Invalid FEN string";
        case ChessError::InvalidMove:
            return "Invalid move";
        case ChessError::InvalidPosition:
            return "Invalid position";
        case ChessError::ParseError:
            return "Parse error";
        case ChessError::OutOfBounds:
            return "Out of bounds";
        case ChessError::IllegalMove:
            return "Illegal move";
        case ChessError::GameOver:
            return "Game over";
    }
    return "Unknown error";
}

// Result type template - similar to std::expected from C++23
template <typename T>
class Result {
   public:
    // Success constructor
    constexpr Result(T value) noexcept(std::is_nothrow_move_constructible_v<T>)
        : data_(std::move(value)) {}

    // Error constructor
    constexpr Result(ChessError error) noexcept : data_(error) {}

    // Check if result contains a value
    [[nodiscard]] constexpr bool hasValue() const noexcept {
        return std::holds_alternative<T>(data_);
    }

    // Check if result contains an error
    [[nodiscard]] constexpr bool hasError() const noexcept {
        return std::holds_alternative<ChessError>(data_);
    }

    // Get the value (undefined behavior if hasError())
    [[nodiscard]] constexpr const T& value() const& noexcept {
        return std::get<T>(data_);
    }

    [[nodiscard]] constexpr T& value() & noexcept {
        return std::get<T>(data_);
    }

    [[nodiscard]] constexpr T&& value() && noexcept {
        return std::get<T>(std::move(data_));
    }

    // Get the error (undefined behavior if hasValue())
    [[nodiscard]] constexpr ChessError error() const noexcept {
        return std::get<ChessError>(data_);
    }

    // Get value or default
    template <typename U>
    [[nodiscard]] constexpr T valueOr(U&& defaultValue) const& {
        return hasValue() ? value() : static_cast<T>(std::forward<U>(defaultValue));
    }

    template <typename U>
    [[nodiscard]] constexpr T valueOr(U&& defaultValue) && {
        return hasValue() ? std::move(*this).value()
                          : static_cast<T>(std::forward<U>(defaultValue));
    }

    // Convenience operators
    [[nodiscard]] constexpr explicit operator bool() const noexcept {
        return hasValue();
    }

    [[nodiscard]] constexpr const T& operator*() const& noexcept {
        return value();
    }

    [[nodiscard]] constexpr T& operator*() & noexcept {
        return value();
    }

    [[nodiscard]] constexpr T&& operator*() && noexcept {
        return std::move(*this).value();
    }

    [[nodiscard]] constexpr const T* operator->() const noexcept {
        return hasValue() ? &value() : nullptr;
    }

    [[nodiscard]] constexpr T* operator->() noexcept {
        return hasValue() ? &value() : nullptr;
    }

   private:
    std::variant<T, ChessError> data_;
};

// Deduction guide
template <typename T>
Result(T) -> Result<T>;

// Helper functions for creating results
template <typename T>
[[nodiscard]] constexpr Result<T> makeSuccess(T&& value) {
    return Result<T>{std::forward<T>(value)};
}

[[nodiscard]] constexpr auto makeError(ChessError error) {
    return [error]<typename T>() -> Result<T> { return Result<T>{error}; };
}

// Specialization for void operations
template <>
class Result<void> {
   public:
    constexpr Result() noexcept : error_(std::nullopt) {}
    constexpr Result(ChessError error) noexcept : error_(error) {}

    [[nodiscard]] constexpr bool hasValue() const noexcept {
        return !error_.has_value();
    }

    [[nodiscard]] constexpr bool hasError() const noexcept {
        return error_.has_value();
    }

    [[nodiscard]] constexpr ChessError error() const noexcept {
        return error_.value();
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept {
        return hasValue();
    }

   private:
    std::optional<ChessError> error_;
};

using VoidResult = Result<void>;

} // namespace chess