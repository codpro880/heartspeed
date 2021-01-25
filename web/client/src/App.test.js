import { render, screen } from '@testing-library/react';
import App from './App';

test('renders Registration page', () => {
  render(<App />);
  const linkElement = screen.getByText("Sign up");
  expect(linkElement).toBeInTheDocument();
});
